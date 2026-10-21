#include "rpg_circlemud_mob_bridge.h"

#include "rpg_catalog.h"
#include "rpg_types.h"
#include "rpg_world.h"

#include "db.h"
#include "structs.h"
#include "utils.h"

/*
 * CircleMUD does not expose a stable per-NPC instance identifier in the
 * legacy char_data ABI.  This adapter therefore represents NPC prototypes
 * by VNUM in the RPG world archetype layer rather than pretending that a
 * VNUM is a unique live instance.
 */
static int circlemud_mob_catalog_index(int vnum) {
  int local;

  if (vnum < 0)
    return -1;

  local = vnum % RPG_CATALOG_MONSTERS;
  if (local < 0)
    local += RPG_CATALOG_MONSTERS;
  return 750 + local;
}

static rpg_mob_tier circlemud_mob_tier(const rpg_catalog_entry *entry) {
  if (!entry)
    return RPG_MOB_NORMAL;

  switch (entry->subtype % 5) {
    case 1: return RPG_MOB_ELITE;
    case 2: return RPG_MOB_RARE;
    case 3: return RPG_MOB_CHAMPION;
    case 4: return RPG_MOB_WORLD_BOSS;
    default: return RPG_MOB_NORMAL;
  }
}

int rpg_circlemud_sync_mob_prototype(struct char_data *mob) {
  int vnum;
  int catalog_index;
  int room_id = -1;
  int zone_id = -1;
  int level;
  double health;
  double damage;
  double armor;
  rpg_catalog_entry entry;

  if (!mob || !IS_NPC(mob) || !IS_MOB(mob))
    return 0;

  vnum = GET_MOB_VNUM(mob);
  catalog_index = circlemud_mob_catalog_index(vnum);
  if (catalog_index < 0 || !rpg_catalog_valid_index(catalog_index))
    return 0;

  entry = rpg_catalog_get(catalog_index);
  level = GET_LEVEL(mob) > 0 ? GET_LEVEL(mob) : entry.level;
  health = GET_MAX_HIT(mob) > 0 ? (double)GET_MAX_HIT(mob) : entry.power * 20.0;
  damage = (double)GET_DAMROLL(mob);
  if (damage <= 0.0)
    damage = entry.power;
  armor = (double)GET_AC(mob);
  if (armor <= 0.0)
    armor = entry.defense;

  if (IN_ROOM(mob) != NOWHERE && IN_ROOM(mob) >= 0 && IN_ROOM(mob) <= top_of_world) {
    room_id = (int)IN_ROOM(mob);
    zone_id = world[IN_ROOM(mob)].zone;
  }

  /* VNUM is the archetype key; this is deliberately not a live-instance ID. */
  if (rpg_world_find(vnum))
    return 1;

  return rpg_world_spawn(vnum, level, circlemud_mob_tier(&entry), 1,
                         zone_id, health, damage, armor);
}

void rpg_circlemud_sync_mob_prototypes(void) {
  struct char_data *mob;

  for (mob = character_list; mob; mob = mob->next)
    if (IS_NPC(mob))
      (void)rpg_circlemud_sync_mob_prototype(mob);
}
