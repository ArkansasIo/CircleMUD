#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "aetherworld_instance.h"

static aw_instance_request request_for(aw_instance_type type) {
  aw_instance_request request;
  memset(&request, 0, sizeof(request));
  request.world_seed = 123456U;
  request.template_id = 17U;
  request.generator_version = 1U;
  request.difficulty_tier = 3;
  request.depth = 5;
  request.min_rooms = 12;
  request.max_rooms = 20;
  request.optional_branch_percent = 40;
  request.encounter_budget = 25;
  request.hazard_budget = 8;
  request.type = type;
  return request;
}

static void test_determinism_and_validation(void) {
  aw_instance_request request = request_for(AW_INSTANCE_DUNGEON);
  aw_instance a, b;
  assert(aw_instance_generate(&request, &a) == AW_INSTANCE_OK);
  assert(aw_instance_generate(&request, &b) == AW_INSTANCE_OK);
  assert(a.seed == b.seed);
  assert(a.room_count == b.room_count);
  assert(a.objective_room_id == b.objective_room_id);
  assert(memcmp(a.rooms, b.rooms,
                (size_t)a.room_count * sizeof(a.rooms[0])) == 0);
  assert(aw_instance_validate(&a) == AW_INSTANCE_OK);
}

static void test_input_bounds_and_budget(void) {
  aw_instance_request request = request_for(AW_INSTANCE_RAID);
  aw_instance instance;
  request.max_rooms = AW_INSTANCE_MAX_ROOMS + 1;
  assert(aw_instance_generate(&request, &instance) == AW_INSTANCE_ERR_RANGE);

  request = request_for(AW_INSTANCE_DUNGEON);
  request.encounter_budget = 0;
  request.hazard_budget = 0;
  assert(aw_instance_generate(&request, &instance) == AW_INSTANCE_OK);
  assert(aw_instance_validate(&instance) == AW_INSTANCE_OK);
  instance.rooms[1].encounter_cost = 1;
  assert(aw_instance_validate(&instance) == AW_INSTANCE_ERR_BUDGET);
}

static void test_invalid_exit_and_duplicate_id(void) {
  aw_instance_request request = request_for(AW_INSTANCE_TRAIL);
  aw_instance instance;
  assert(aw_instance_generate(&request, &instance) == AW_INSTANCE_OK);
  instance.rooms[0].exits[0] = 65500U;
  assert(aw_instance_validate(&instance) == AW_INSTANCE_ERR_EXIT);

  assert(aw_instance_generate(&request, &instance) == AW_INSTANCE_OK);
  instance.rooms[1].id = instance.rooms[0].id;
  assert(aw_instance_validate(&instance) == AW_INSTANCE_ERR_DUPLICATE_ID);
}

static void test_lifecycle(void) {
  aw_instance_request request = request_for(AW_INSTANCE_DUNGEON);
  aw_instance instance;
  assert(aw_instance_generate(&request, &instance) == AW_INSTANCE_OK);
  assert(instance.state == AW_INSTANCE_VALIDATED);
  assert(aw_instance_transition(&instance, AW_INSTANCE_ACTIVE) == AW_INSTANCE_OK);
  assert(aw_instance_transition(&instance, AW_INSTANCE_COMPLETED) == AW_INSTANCE_OK);
  assert(aw_instance_transition(&instance, AW_INSTANCE_ACTIVE) == AW_INSTANCE_ERR_STATE);
  assert(aw_instance_transition(&instance, AW_INSTANCE_ARCHIVED) == AW_INSTANCE_OK);
}

static void test_tower_reward_idempotency_and_progression(void) {
  aw_tower_run run;
  assert(aw_tower_run_init(&run, 99, 456, 2) == AW_INSTANCE_OK);
  assert(run.current_floor == 1);
  assert(aw_tower_complete_floor(&run, 1001) == AW_INSTANCE_OK);
  assert(run.current_floor == 2);
  assert(aw_tower_complete_floor(&run, 1001) == AW_INSTANCE_ERR_STATE);
  assert(run.current_floor == 2);
  assert(aw_tower_complete_floor(&run, 1002) == AW_INSTANCE_OK);
  assert(run.state == AW_INSTANCE_COMPLETED);
  assert(aw_tower_complete_floor(&run, 1003) == AW_INSTANCE_ERR_STATE);
}

int main(void) {
  test_determinism_and_validation();
  test_input_bounds_and_budget();
  test_invalid_exit_and_duplicate_id();
  test_lifecycle();
  test_tower_reward_idempotency_and_progression();
  puts("AetherWorld instance tests passed");
  return 0;
}
