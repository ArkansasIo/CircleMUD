#!/usr/bin/env python3
"""Generate the deterministic 900-entry CircleMUD RPG catalog.

Outputs one JSON document containing 150 weapons, 150 armor pieces,
200 items, 150 consumables, 100 materials, 100 monsters, and 50 NPCs.
No third-party packages are required.
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SPEC = ROOT / "data" / "rpg" / "catalog_900.json"
DEFAULT_OUT = ROOT / "data" / "rpg" / "catalog_900.generated.json"

COUNTS = {"weapons": 150, "armor": 150, "items": 200, "consumables": 150,
          "materials": 100, "monsters": 100, "npcs": 50}
RARITIES = ["Common", "Uncommon", "Rare", "Epic", "Legendary", "Mythic", "Ancient", "Transcendent", "Eternal"]

def rarity_for(index, count):
    return RARITIES[min(8, (index * 9) // count)]

def build(spec):
    prefixes = spec["naming"]["prefixes"]
    cats = spec["categories"]
    entries = []
    serial = 1

    def add(category, index, count, name, extra):
        nonlocal serial
        level = ((index * 7) + serial * 3) % 720 + 1
        rec = {
            "id": f"{category[:3].upper()}-{serial:04d}",
            "category": category,
            "name": name,
            "level": level,
            "rarity": rarity_for(index, count),
        }
        rec.update(extra)
        entries.append(rec)
        serial += 1

    weapon_types = cats["weapons"]["types"]
    for i in range(COUNTS["weapons"]):
        t = weapon_types[i % len(weapon_types)]
        p = prefixes[i % len(prefixes)]
        add("weapon", i, COUNTS["weapons"], f"{p} {t.title()} {i+1:03d}", {
            "weapon_type": t,
            "slot": "ranged" if t in {"bow", "crossbow", "staff", "wand", "tome"} else "main_hand",
            "damage_type": cats["weapons"]["damage_types"][i % len(cats["weapons"]["damage_types"])],
            "power_scale": round(1.0 + (i % 9) * 0.12, 3),
            "affix_slots": min(8, i // 18),
        })

    armor_types = cats["armor"]["slots"]
    weights = cats["armor"]["weights"]
    for i in range(COUNTS["armor"]):
        slot = armor_types[i % len(armor_types)]
        p = prefixes[(i + 3) % len(prefixes)]
        add("armor", i, COUNTS["armor"], f"{p} {slot.title()} {i+1:03d}", {
            "armor_slot": slot,
            "weight_class": weights[i % len(weights)],
            "armor_scale": round(1.0 + (i % 9) * 0.10, 3),
            "resistance_scale": round(0.5 + (i % 6) * 0.15, 3),
        })

    item_types = cats["items"]["types"]
    for i in range(COUNTS["items"]):
        t = item_types[i % len(item_types)]
        p = prefixes[(i + 5) % len(prefixes)]
        add("item", i, COUNTS["items"], f"{p} {t.title()} {i+1:03d}", {
            "item_type": t,
            "use": ["quest", "utility", "relic", "key", "crafting"][i % 5],
            "stack_max": [1, 5, 10, 20, 99][i % 5],
        })

    effects = cats["consumables"]["effects"]
    con_types = cats["consumables"]["types"]
    for i in range(COUNTS["consumables"]):
        t = con_types[i % len(con_types)]
        effect = effects[i % len(effects)]
        p = prefixes[(i + 7) % len(prefixes)]
        add("consumable", i, COUNTS["consumables"], f"{p} {t.title()} {i+1:03d}", {
            "consumable_type": t,
            "effect": effect,
            "magnitude_scale": round(1.0 + (i % 12) * 0.125, 3),
            "stack_max": 20,
        })

    mat_types = cats["materials"]["types"]
    sources = cats["materials"]["sources"]
    for i in range(COUNTS["materials"]):
        t = mat_types[i % len(mat_types)]
        p = prefixes[(i + 11) % len(prefixes)]
        add("material", i, COUNTS["materials"], f"{p} {t.title()} {i+1:03d}", {
            "material_type": t,
            "gather_source": sources[i % len(sources)],
            "crafting_tier": 1 + (i % 9),
            "stack_max": 999,
        })

    monster_types = cats["monsters"]["types"]
    tiers = cats["monsters"]["tiers"]
    for i in range(COUNTS["monsters"]):
        t = monster_types[i % len(monster_types)]
        tier = tiers[i % len(tiers)]
        p = prefixes[(i + 13) % len(prefixes)]
        add("monster", i, COUNTS["monsters"], f"{p} {t.title()} {i+1:03d}", {
            "monster_type": t,
            "tier": tier,
            "spawn_type": ["static", "patrol", "ambush", "event", "dynamic"][i % 5],
            "health_scale": round(1.0 + (i % 5) * 0.20, 3),
            "damage_scale": round(1.0 + (i % 5) * 0.15, 3),
            "loot_table": "world_mob" if tier == "normal" else "elite_mob" if tier == "elite" else "world_boss" if tier == "world_boss" else "elite_mob",
        })

    roles = cats["npcs"]["roles"]
    factions = cats["npcs"]["factions"]
    for i in range(COUNTS["npcs"]):
        role = roles[i % len(roles)]
        faction = factions[i % len(factions)]
        p = prefixes[(i + 17) % len(prefixes)]
        add("npc", i, COUNTS["npcs"], f"{p} {role.title()} {i+1:03d}", {
            "role": role,
            "faction": faction,
            "services": [role, "dialogue", "quest", "vendor"][0:1 + (i % 4)],
        })

    assert len(entries) == 900, len(entries)
    return {
        "schema_version": "1.0",
        "catalog_id": "circlemud-900-content",
        "generated": True,
        "total_entries": len(entries),
        "distribution": COUNTS,
        "entries": entries,
    }

def main():
    output = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_OUT
    spec = json.loads(SPEC.read_text(encoding="utf-8"))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(build(spec), indent=2) + "\n", encoding="utf-8")
    print(f"generated {output} with 900 entries")

if __name__ == "__main__":
    main()
