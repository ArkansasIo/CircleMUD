#include "rpg_npc.h"

int rpg_npc_init(rpg_npc *npc, int id, int catalog_index, int zone_id, int faction_id) {
  rpg_catalog_entry e;
  if (!npc || catalog_index < 850 || catalog_index >= 900) return 0;
  e = rpg_catalog_get(catalog_index);
  npc->id = id;
  npc->catalog_index = catalog_index;
  npc->zone_id = zone_id;
  npc->role = (rpg_npc_role)((catalog_index - 850) % 10);
  npc->faction_id = faction_id;
  npc->level = e.level;
  npc->active = 1;
  npc->health = 100.0 + e.power * 50.0;
  npc->armor = e.power * 2.0;
  return 1;
}

int rpg_npc_can_service(const rpg_npc *npc, rpg_npc_role role) {
  return npc && npc->active && npc->role == role;
}
