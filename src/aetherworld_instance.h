#ifndef AETHERWORLD_INSTANCE_H
#define AETHERWORLD_INSTANCE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AW_INSTANCE_MAX_ROOMS 128
#define AW_INSTANCE_MAX_EXITS 4
#define AW_INSTANCE_MAX_RETRIES 8
#define AW_TOWER_MAX_FLOORS 100

typedef enum aw_instance_type {
  AW_INSTANCE_DUNGEON = 1,
  AW_INSTANCE_RAID = 2,
  AW_INSTANCE_TRAIL = 3,
  AW_INSTANCE_TOWER = 4
} aw_instance_type;

typedef enum aw_room_role {
  AW_ROOM_START = 1,
  AW_ROOM_NORMAL = 2,
  AW_ROOM_BRANCH = 3,
  AW_ROOM_KEY = 4,
  AW_ROOM_BOSS = 5,
  AW_ROOM_EXIT = 6,
  AW_ROOM_REST = 7
} aw_room_role;

typedef enum aw_instance_state {
  AW_INSTANCE_CREATED = 1,
  AW_INSTANCE_VALIDATED = 2,
  AW_INSTANCE_ACTIVE = 3,
  AW_INSTANCE_COMPLETED = 4,
  AW_INSTANCE_FAILED = 5,
  AW_INSTANCE_ABANDONED = 6,
  AW_INSTANCE_REJECTED = 7,
  AW_INSTANCE_ARCHIVED = 8
} aw_instance_state;

typedef struct aw_instance_request {
  uint32_t world_seed;
  uint32_t template_id;
  uint32_t generator_version;
  uint16_t difficulty_tier;
  uint16_t depth;
  uint16_t min_rooms;
  uint16_t max_rooms;
  uint16_t optional_branch_percent;
  uint16_t hazard_budget;
  uint16_t encounter_budget;
  aw_instance_type type;
} aw_instance_request;

typedef struct aw_instance_room {
  uint16_t id;
  aw_room_role role;
  uint16_t exits[AW_INSTANCE_MAX_EXITS];
  uint8_t exit_count;
  uint8_t encounter_cost;
  uint8_t hazard_cost;
  uint8_t flags;
} aw_instance_room;

typedef struct aw_instance {
  uint32_t seed;
  uint32_t template_id;
  uint32_t generator_version;
  uint16_t difficulty_tier;
  uint16_t depth;
  uint16_t room_count;
  uint16_t start_room_id;
  uint16_t objective_room_id;
  uint16_t encounter_budget;
  uint16_t hazard_budget;
  aw_instance_type type;
  aw_instance_state state;
  aw_instance_room rooms[AW_INSTANCE_MAX_ROOMS];
} aw_instance;

typedef struct aw_tower_run {
  uint64_t run_id;
  uint32_t world_seed;
  uint16_t current_floor;
  uint16_t max_floor;
  aw_instance_state state;
  uint64_t last_reward_transaction_id;
} aw_tower_run;

typedef enum aw_instance_error {
  AW_INSTANCE_OK = 0,
  AW_INSTANCE_ERR_ARGUMENT = 1,
  AW_INSTANCE_ERR_RANGE = 2,
  AW_INSTANCE_ERR_NO_PATH = 3,
  AW_INSTANCE_ERR_EXIT = 4,
  AW_INSTANCE_ERR_DUPLICATE_ID = 5,
  AW_INSTANCE_ERR_BUDGET = 6,
  AW_INSTANCE_ERR_STATE = 7,
  AW_INSTANCE_ERR_GENERATION = 8,
  AW_INSTANCE_ERR_NOT_FOUND = 9
} aw_instance_error;

/* Pure deterministic PRNG utility; no process-global RNG state is used. */
uint32_t aw_instance_next_random(uint32_t *state);
uint32_t aw_instance_derive_seed(const aw_instance_request *request);

/* Build a validated logical room graph. Does not touch CircleMUD world data. */
aw_instance_error aw_instance_generate(const aw_instance_request *request,
                                        aw_instance *out);
aw_instance_error aw_instance_validate(const aw_instance *instance);
const aw_instance_room *aw_instance_find_room(const aw_instance *instance,
                                              uint16_t room_id);

/* Explicit lifecycle transitions are enforced by the module. */
aw_instance_error aw_instance_transition(aw_instance *instance,
                                         aw_instance_state next_state);

/* Tower progression has exactly-once reward transaction semantics. */
aw_instance_error aw_tower_run_init(aw_tower_run *run, uint64_t run_id,
                                    uint32_t world_seed, uint16_t max_floor);
aw_instance_error aw_tower_complete_floor(aw_tower_run *run,
                                          uint64_t reward_transaction_id);
aw_instance_error aw_tower_fail(aw_tower_run *run);
aw_instance_error aw_tower_abandon(aw_tower_run *run);

#ifdef __cplusplus
}
#endif

#endif
