#include "rpg_circlemud_mob_bridge.h"

#include "conf.h"
#include "sysdep.h"

#include "rpg_catalog.h"
#include "rpg_live.h"
#include "rpg_types.h"
#include "rpg_world.h"

#include "db.h"
#include "structs.h"
#include "utils.h"

/*
 * CircleMUD does not expose a stable persistent per-NPC instance identifier in
 * the legacy char_data ABI. The bridge therefore derives a reconciliation key
 * from the prototype RNUM/VNUM, room, and deterministic occurrence ordinal.
 * This prevents duplicate RPG instances during ordinary heartbeat passes while
 * avoiding retention of a char_data pointer. A future host integration can
 * replace the derived key with a true instance ID without changing RPG state.
 */
static int circlemud_mob_source_key(const struct char_data *mob, int ordinal) {
  long long key;
  int room = -1;
  int rnum;
  int vnum;

  if (!mob || ordinal < 0)
    return 0;

  vnum = GET_MOB_VNUM(mob);
  rnum = GET_MOB_RNUM(mob);
  if (IN_ROOM(mob) != NOWHERE)
    room = (int)IN_ROOM(mob);

  key = ((long long)(rnum + 1) * 1000003LL) ^
        ((long long)(vnum + 1) * 9176LL) ^
        ((long long)(room + 2) * 131LL) ^
        ((long long)(ordinal + 1) * 17LL);
  key &= 0x7fffffffLL;
  return key > 0 ? (int)key : 1;
}

static int circlemud_mob_catalog_index(int vnum) {
  int local;

  if (vnum < 0)
    return -1;

  local = vnum % RPG_CATALOG_MONSTERS;
  if (local < 0)
    local += RPG_CATALOG_MONSTERS;
  return 750 + local;
}

int rpg_circlemud_sync_mob_prototype(struct char_data *mob) {
  int vnum;
  int catalog_index;
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

  if (IN_ROOM(mob) != NOWHERE && IN_ROOM(mob) >= 0 && IN_ROOM(mob) <= top_of_world)
    zone_id = world[IN_ROOM(mob)].zone;

  return rpg_live_sync_monster_source(circlemud_mob_source_key(mob, 0),
                                      catalog_index, zone_id, 1, level,
                                      health, health, damage, armor);
}

void rpg_circlemud_sync_mob_prototypes(void) {
  struct char_data *mob;
  int ordinal = 0;

  rpg_live_begin_monster_sync();

  for (mob = character_list; mob; mob = mob->next) {
    int vnum;
    int catalog_index;
    int zone_id = -1;
    int level;
    double health;
    double damage;
    double armor;
    rpg_catalog_entry entry;

    if (!IS_NPC(mob) || !IS_MOB(mob))
      continue;

    vnum = GET_MOB_VNUM(mob);
    catalog_index = circlemud_mob_catalog_index(vnum);
    if (catalog_index < 0 || !rpg_catalog_valid_index(catalog_index))
      continue;

    entry = rpg_catalog_get(catalog_index);
    level = GET_LEVEL(mob) > 0 ? GET_LEVEL(mob) : entry.level;
    health = GET_MAX_HIT(mob) > 0 ? (double)GET_MAX_HIT(mob) : entry.power * 20.0;
    damage = (double)GET_DAMROLL(mob);
    if (damage <= 0.0)
      damage = entry.power;
    armor = (double)GET_AC(mob);
    if (armor <= 0.0)
      armor = entry.defense;

    if (IN_ROOM(mob) != NOWHERE && IN_ROOM(mob) >= 0 && IN_ROOM(mob) <= top_of_world)
      zone_id = world[IN_ROOM(mob)].zone;

    ++ordinal;
    (void)rpg_live_sync_monster_source(
        circlemud_mob_source_key(mob, ordinal), catalog_index, zone_id, 1,
        level, health, health, damage, armor);
  }

  rpg_live_end_monster_sync();
}

struct char_data *rpg_circlemud_find_mob_by_source_key(int source_key) {
  struct char_data *mob;
  int ordinal = 0;

  if (source_key <= 0)
    return NULL;

  for (mob = character_list; mob; mob = mob->next) {
    if (!IS_NPC(mob) || !IS_MOB(mob))
      continue;

    ++ordinal;
    if (circlemud_mob_source_key(mob, ordinal) == source_key)
      return mob;
  }

  return NULL;
}
