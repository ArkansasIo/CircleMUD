#ifndef CIRCLEMUD_RPG_NPC_H
#define CIRCLEMUD_RPG_NPC_H

#include "rpg_catalog.h"

typedef enum {
  RPG_NPC_VENDOR = 0,
  RPG_NPC_QUEST_GIVER,
  RPG_NPC_TRAINER,
  RPG_NPC_GUARD,
  RPG_NPC_CRAFTER,
  RPG_NPC_BANKER,
  RPG_NPC_GUILD,
  RPG_NPC_STORY,
  RPG_NPC_FOLLOWER,
  RPG_NPC_WORLD_EVENT
} rpg_npc_role;

typedef struct {
  int id;
  int catalog_index;
  int zone_id;
  rpg_npc_role role;
  int faction_id;
  int level;
  int active;
  double health;
  double armor;
} rpg_npc;

int rpg_npc_init(rpg_npc *npc, int id, int catalog_index, int zone_id, int faction_id);
int rpg_npc_can_service(const rpg_npc *npc, rpg_npc_role role);

#endif
