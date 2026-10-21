#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "aetherworld_instance_manager.h"

static aw_instance_request request(void) {
  aw_instance_request r;
  memset(&r, 0, sizeof(r));
  r.world_seed = 777;
  r.template_id = 3;
  r.generator_version = 1;
  r.difficulty_tier = 2;
  r.depth = 1;
  r.min_rooms = 8;
  r.max_rooms = 12;
  r.optional_branch_percent = 30;
  r.encounter_budget = 12;
  r.hazard_budget = 5;
  r.type = AW_INSTANCE_DUNGEON;
  return r;
}

int main(void) {
  aw_instance_manager manager;
  aw_managed_instance *slot;
  uint64_t id;
  aw_instance_manager_init(&manager);
  assert(aw_instance_manager_active_count(&manager) == 0);
  assert(aw_instance_manager_create(&manager, &request(), 2, &id) == AW_MANAGER_OK);
  assert(aw_instance_manager_active_count(&manager) == 1);
  assert(aw_instance_manager_find(&manager, id, &slot) == AW_MANAGER_OK);
  assert(slot->instance.state == AW_INSTANCE_VALIDATED);
  assert(aw_instance_manager_start(&manager, id) == AW_MANAGER_OK);
  assert(aw_instance_manager_join(&manager, id, 101) == AW_MANAGER_OK);
  assert(aw_instance_manager_join(&manager, id, 101) == AW_MANAGER_ERR_PLAYER);
  assert(aw_instance_manager_join(&manager, id, 102) == AW_MANAGER_OK);
  assert(aw_instance_manager_join(&manager, id, 103) == AW_MANAGER_ERR_CAPACITY);
  assert(aw_instance_manager_leave(&manager, id, 101) == AW_MANAGER_OK);
  assert(!aw_instance_manager_has_player(slot, 101));
  assert(aw_instance_manager_join(&manager, id, 103) == AW_MANAGER_OK);
  assert(aw_instance_manager_complete(&manager, id) == AW_MANAGER_OK);
  assert(aw_instance_manager_destroy(&manager, id) == AW_MANAGER_ERR_STATE);
  assert(aw_instance_manager_leave(&manager, id, 102) == AW_MANAGER_OK);
  assert(aw_instance_manager_leave(&manager, id, 103) == AW_MANAGER_OK);
  assert(aw_instance_manager_destroy(&manager, id) == AW_MANAGER_OK);
  assert(aw_instance_manager_active_count(&manager) == 0);
  puts("AetherWorld instance manager tests passed");
  return 0;
}
