#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_circlemud_combat_bridge.h"
#include "../src/rpg/rpg_circlemud_host_adapter.h"
#include "../src/rpg/rpg_circlemud_host_dispatch.h"
#include "../src/rpg/rpg_live.h"

static int damage_calls;
static int xp_calls;
static int last_player;
static int last_monster;
static int last_amount;

static int test_damage(int player_id, int monster_id, int source_key, int amount) {
  (void)source_key;
  ++damage_calls;
  last_player = player_id;
  last_monster = monster_id;
  last_amount = amount;
  return 1;
}

static int test_death(int player_id, int monster_id, int source_key) {
  (void)player_id;
  (void)monster_id;
  (void)source_key;
  return 1;
}

static int test_xp(int player_id, int monster_id, int amount) {
  (void)monster_id;
  ++xp_calls;
  last_player = player_id;
  last_amount = amount;
  return 1;
}

static int test_loot(int player_id, int monster_id, int catalog_index) {
  (void)player_id;
  (void)monster_id;
  (void)catalog_index;
  return 1;
}

int main(void) {
  rpg_circlemud_host_callbacks callbacks;
  int monster_id;

  rpg_live_init();
  rpg_circlemud_combat_bridge_init();
  rpg_circlemud_host_adapter_init();

  assert(rpg_live_register_player(7, 5) == 1);
  monster_id = rpg_live_spawn_monster(0, 1, 1);
  assert(monster_id > 0);

  callbacks.apply_damage = test_damage;
  callbacks.apply_death = test_death;
  callbacks.grant_xp = test_xp;
  callbacks.materialize_loot = test_loot;
  assert(rpg_circlemud_host_adapter_register(&callbacks) == 1);

  assert(rpg_circlemud_queue_damage(7, monster_id, 0, 20.0) == 1);
  assert(rpg_circlemud_queue_xp(7, monster_id, 40.0) == 1);
  assert(rpg_circlemud_host_dispatch_queue() == 2);
  assert(rpg_circlemud_combat_event_count() == 0);
  assert(damage_calls == 1);
  assert(xp_calls == 1);
  assert(last_player == 7);
  assert(last_monster == monster_id);
  assert(last_amount == 40);

  /* A rejected event remains queued instead of being silently discarded. */
  assert(rpg_circlemud_queue_damage(9999, monster_id, 0, 5.0) == 1);
  assert(rpg_circlemud_host_dispatch_queue() == 0);
  assert(rpg_circlemud_combat_event_count() == 1);
  rpg_circlemud_combat_bridge_clear();

  printf("rpg CircleMUD host dispatch tests passed\n");
  return 0;
}
