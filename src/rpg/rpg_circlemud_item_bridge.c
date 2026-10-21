#include "rpg_circlemud_item_bridge.h"

#include "rpg_catalog.h"
#include "rpg_equipment.h"
#include "rpg_live.h"

#include "structs.h"
#include "utils.h"

static int circlemud_catalog_index(const struct obj_data *obj) {
  int vnum;
  if (!obj)
    return -1;
  vnum = (int)GET_OBJ_VNUM(obj);
  return rpg_catalog_valid_index(vnum) ? vnum : -1;
}

static int equipment_slot_for_wear_position(int wear_position) {
  switch (wear_position) {
    case WEAR_HEAD: return RPG_SLOT_HEAD;
    case WEAR_BODY: return RPG_SLOT_CHEST;
    case WEAR_LEGS: return RPG_SLOT_LEGS;
    case WEAR_FEET: return RPG_SLOT_FEET;
    case WEAR_HANDS: return RPG_SLOT_HANDS;
    case WEAR_ABOUT: return RPG_SLOT_CLOAK;
    case WEAR_WAIST: return RPG_SLOT_BELT;
    case WEAR_WRIST_R:
    case WEAR_WRIST_L: return RPG_SLOT_BRACERS;
    case WEAR_NECK_1:
    case WEAR_NECK_2: return RPG_SLOT_AMULET;
    case WEAR_FINGER_R:
    case WEAR_FINGER_L: return RPG_SLOT_RING;
    default: return -1;
  }
}

static int add_object(rpg_live_player *player, const struct obj_data *obj,
                      int *next_index) {
  int catalog_index;
  int index;

  if (!player || !obj || !next_index || *next_index >= RPG_LIVE_MAX_INVENTORY)
    return -1;

  catalog_index = circlemud_catalog_index(obj);
  if (catalog_index < 0)
    return -1;

  index = *next_index;
  if (!rpg_live_set_player_item(player, index, catalog_index, 1, 100.0))
    return -1;

  ++(*next_index);
  return index;
}

int rpg_circlemud_sync_items(struct char_data *ch) {
  rpg_live_player *player;
  struct obj_data *obj;
  int next_index = 0;
  int wear_position;

  if (!ch || IS_NPC(ch))
    return 0;

  player = rpg_live_get_player((int)ch->char_specials.saved.idnum);
  if (!player)
    return 0;

  /* RPG equipment pointers always target RPG-owned item storage. */
  rpg_equipment_init(&player->character.equipment);
  player->equipment_count = 0;
  player->inventory_count = 0;

  for (obj = ch->carrying; obj && next_index < RPG_LIVE_MAX_INVENTORY;
       obj = obj->next_content)
    (void)add_object(player, obj, &next_index);

  for (wear_position = 0;
       wear_position < NUM_WEARS && next_index < RPG_LIVE_MAX_INVENTORY;
       ++wear_position) {
    int slot;
    int index;

    obj = GET_EQ(ch, wear_position);
    if (!obj)
      continue;

    slot = equipment_slot_for_wear_position(wear_position);
    if (slot < 0)
      continue;

    /* A two-ring/two-neck/two-wrist legacy layout maps to one RPG slot. */
    if (player->equipment_item_index[slot] >= 0)
      continue;

    index = add_object(player, obj, &next_index);
    if (index < 0)
      continue;

    if (rpg_live_set_player_equipment_slot(player, slot, index))
      player->character.equipment.slots[slot] = &player->inventory[index];
  }

  player->inventory_count = next_index;
  rpg_character_recalculate(&player->character);
  return 1;
}
