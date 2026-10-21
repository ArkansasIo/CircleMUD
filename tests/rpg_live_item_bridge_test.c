#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_live.h"

int main(void) {
  rpg_live_player *player;

  rpg_live_init();
  assert(rpg_live_register_player(1001, 10) == 1);

  player = rpg_live_get_player(1001);
  assert(player != NULL);
  assert(player->room_id == -1);
  assert(player->zone_id == -1);

  assert(rpg_live_set_player_world_state(1001, 42, 7, 2, 1, 15) == 1);
  assert(player->room_id == 42);
  assert(player->zone_id == 7);
  assert(player->inventory_count == 2);
  assert(player->equipment_count == 1);
  assert(player->inventory_weight == 15);

  assert(rpg_live_set_player_item(player, 0, 0, 1, 100.0) == 1);
  assert(player->inventory[0].catalog.index == 0);
  assert(player->inventory[0].stack == 1);
  assert(rpg_live_set_player_equipment_slot(player, RPG_SLOT_HEAD, 0) == 1);
  assert(player->equipment_item_index[RPG_SLOT_HEAD] == 0);
  assert(player->equipment_count == 1);

  assert(rpg_live_set_player_item(player, RPG_LIVE_MAX_INVENTORY, 0, 1, 100.0) == 0);
  assert(rpg_live_set_player_item(player, 1, RPG_CATALOG_TOTAL, 1, 100.0) == 0);
  assert(rpg_live_set_player_equipment_slot(player, RPG_SLOT_CHEST, 99) == 0);

  assert(rpg_live_unregister_player(1001) == 1);
  assert(rpg_live_get_player(1001) == NULL);

  printf("rpg live item bridge tests passed\n");
  return 0;
}
