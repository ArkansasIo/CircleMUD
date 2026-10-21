#include "aetherworld_instance.h"

#include <string.h>

static uint32_t aw_mix32(uint32_t x) {
  x ^= x >> 16;
  x *= UINT32_C(0x7feb352d);
  x ^= x >> 15;
  x *= UINT32_C(0x846ca68b);
  x ^= x >> 16;
  return x;
}

uint32_t aw_instance_next_random(uint32_t *state) {
  uint32_t x;
  if (state == NULL) return 0;
  x = *state;
  if (x == 0) x = UINT32_C(0x6d2b79f5);
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

uint32_t aw_instance_derive_seed(const aw_instance_request *request) {
  uint32_t seed;
  if (request == NULL) return 0;
  seed = aw_mix32(request->world_seed ^ UINT32_C(0x9e3779b9));
  seed ^= aw_mix32(request->template_id + UINT32_C(0x85ebca6b));
  seed ^= aw_mix32(request->generator_version + UINT32_C(0xc2b2ae35));
  seed ^= aw_mix32(((uint32_t)request->difficulty_tier << 16) | request->depth);
  seed ^= aw_mix32(((uint32_t)request->type << 24) |
                   ((uint32_t)request->min_rooms << 12) |
                   (uint32_t)request->max_rooms);
  seed ^= aw_mix32(((uint32_t)request->optional_branch_percent << 16) |
                   request->hazard_budget);
  seed ^= aw_mix32((uint32_t)request->encounter_budget);
  seed = aw_mix32(seed);
  return seed == 0 ? UINT32_C(0x6d2b79f5) : seed;
}

const aw_instance_room *aw_instance_find_room(const aw_instance *instance,
                                              uint16_t room_id) {
  uint16_t i;
  if (instance == NULL || room_id == 0) return NULL;
  for (i = 0; i < instance->room_count && i < AW_INSTANCE_MAX_ROOMS; ++i)
    if (instance->rooms[i].id == room_id) return &instance->rooms[i];
  return NULL;
}

static int aw_reachable(const aw_instance *instance, uint16_t target) {
  uint16_t queue[AW_INSTANCE_MAX_ROOMS];
  uint8_t visited[AW_INSTANCE_MAX_ROOMS];
  size_t head = 0, tail = 0;
  uint16_t i;
  memset(visited, 0, sizeof(visited));
  for (i = 0; i < instance->room_count; ++i) {
    if (instance->rooms[i].id == instance->start_room_id) {
      queue[tail++] = instance->start_room_id;
      visited[i] = 1;
      break;
    }
  }
  if (tail == 0) return 0;
  while (head < tail) {
    uint16_t id = queue[head++];
    const aw_instance_room *room = aw_instance_find_room(instance, id);
    uint8_t e;
    if (id == target) return 1;
    if (room == NULL) return 0;
    for (e = 0; e < room->exit_count; ++e) {
      uint16_t j;
      uint16_t next = room->exits[e];
      for (j = 0; j < instance->room_count; ++j) {
        if (instance->rooms[j].id == next && !visited[j]) {
          visited[j] = 1;
          if (tail >= AW_INSTANCE_MAX_ROOMS) return 0;
          queue[tail++] = next;
          break;
        }
      }
    }
  }
  return 0;
}

aw_instance_error aw_instance_validate(const aw_instance *instance) {
  uint32_t encounter_total = 0, hazard_total = 0;
  uint16_t i, j;
  const aw_instance_room *start, *objective;
  if (instance == NULL) return AW_INSTANCE_ERR_ARGUMENT;
  if (instance->room_count < 2 || instance->room_count > AW_INSTANCE_MAX_ROOMS)
    return AW_INSTANCE_ERR_RANGE;
  if (instance->generator_version == 0 || instance->seed == 0)
    return AW_INSTANCE_ERR_ARGUMENT;
  if (instance->type < AW_INSTANCE_DUNGEON || instance->type > AW_INSTANCE_TOWER)
    return AW_INSTANCE_ERR_RANGE;
  start = aw_instance_find_room(instance, instance->start_room_id);
  objective = aw_instance_find_room(instance, instance->objective_room_id);
  if (start == NULL || objective == NULL || start == objective)
    return AW_INSTANCE_ERR_NOT_FOUND;
  if (start->role != AW_ROOM_START) return AW_INSTANCE_ERR_ARGUMENT;
  if (objective->role != AW_ROOM_BOSS && objective->role != AW_ROOM_EXIT)
    return AW_INSTANCE_ERR_ARGUMENT;
  for (i = 0; i < instance->room_count; ++i) {
    const aw_instance_room *room = &instance->rooms[i];
    if (room->id == 0 || room->exit_count > AW_INSTANCE_MAX_EXITS)
      return AW_INSTANCE_ERR_RANGE;
    for (j = (uint16_t)(i + 1); j < instance->room_count; ++j)
      if (room->id == instance->rooms[j].id)
        return AW_INSTANCE_ERR_DUPLICATE_ID;
    encounter_total += room->encounter_cost;
    hazard_total += room->hazard_cost;
    for (uint8_t e = 0; e < room->exit_count; ++e) {
      const aw_instance_room *dest = aw_instance_find_room(instance, room->exits[e]);
      uint8_t back;
      int bidirectional = 0;
      if (dest == NULL || room->exits[e] == room->id) return AW_INSTANCE_ERR_EXIT;
      for (back = 0; back < dest->exit_count; ++back)
        if (dest->exits[back] == room->id) { bidirectional = 1; break; }
      if (!bidirectional) return AW_INSTANCE_ERR_EXIT;
    }
  }
  if (encounter_total > instance->encounter_budget ||
      hazard_total > instance->hazard_budget)
    return AW_INSTANCE_ERR_BUDGET;
  if (!aw_reachable(instance, instance->objective_room_id))
    return AW_INSTANCE_ERR_NO_PATH;
  return AW_INSTANCE_OK;
}

static int aw_add_exit(aw_instance_room *a, uint16_t b_id) {
  uint8_t i;
  if (a->exit_count >= AW_INSTANCE_MAX_EXITS) return 0;
  for (i = 0; i < a->exit_count; ++i)
    if (a->exits[i] == b_id) return 1;
  a->exits[a->exit_count++] = b_id;
  return 1;
}

aw_instance_error aw_instance_generate(const aw_instance_request *request,
                                        aw_instance *out) {
  aw_instance temp;
  uint32_t rng;
  uint16_t count, i;
  uint32_t encounter_remaining, hazard_remaining;
  if (request == NULL || out == NULL) return AW_INSTANCE_ERR_ARGUMENT;
  if (request->type < AW_INSTANCE_DUNGEON || request->type > AW_INSTANCE_TOWER ||
      request->generator_version == 0)
    return AW_INSTANCE_ERR_RANGE;
  if (request->min_rooms < 2 || request->max_rooms < request->min_rooms ||
      request->max_rooms > AW_INSTANCE_MAX_ROOMS ||
      request->optional_branch_percent > 100 ||
      request->depth == 0 ||
      (request->type == AW_INSTANCE_TOWER && request->depth > AW_TOWER_MAX_FLOORS))
    return AW_INSTANCE_ERR_RANGE;

  memset(&temp, 0, sizeof(temp));
  temp.seed = aw_instance_derive_seed(request);
  temp.template_id = request->template_id;
  temp.generator_version = request->generator_version;
  temp.difficulty_tier = request->difficulty_tier;
  temp.depth = request->depth;
  temp.type = request->type;
  temp.state = AW_INSTANCE_CREATED;
  temp.encounter_budget = request->encounter_budget;
  temp.hazard_budget = request->hazard_budget;
  rng = temp.seed;
  count = (uint16_t)(request->min_rooms +
    (aw_instance_next_random(&rng) % (request->max_rooms - request->min_rooms + 1)));
  temp.room_count = count;
  temp.start_room_id = 1;
  temp.objective_room_id = count;
  for (i = 0; i < count; ++i) {
    aw_instance_room *room = &temp.rooms[i];
    room->id = (uint16_t)(i + 1);
    room->role = (i == 0) ? AW_ROOM_START :
                 (i == count - 1) ? AW_ROOM_BOSS : AW_ROOM_NORMAL;
    if (i > 0) {
      if (!aw_add_exit(&temp.rooms[i - 1], room->id) ||
          !aw_add_exit(room, temp.rooms[i - 1].id))
        return AW_INSTANCE_ERR_GENERATION;
    }
  }

  /* Optional cross-links make loops while preserving a reachable backbone. */
  for (i = 1; i + 1 < count; ++i) {
    uint16_t candidate;
    if (request->optional_branch_percent == 0 ||
        aw_instance_next_random(&rng) % 100 >= request->optional_branch_percent)
      continue;
    candidate = (uint16_t)(aw_instance_next_random(&rng) % (count - 1));
    if (candidate == i || candidate + 1 == i) continue;
    if (temp.rooms[i].exit_count < AW_INSTANCE_MAX_EXITS &&
        temp.rooms[candidate].exit_count < AW_INSTANCE_MAX_EXITS) {
      if (!aw_add_exit(&temp.rooms[i], temp.rooms[candidate].id) ||
          !aw_add_exit(&temp.rooms[candidate], temp.rooms[i].id))
        return AW_INSTANCE_ERR_GENERATION;
      if (candidate > i + 1) temp.rooms[candidate].role = AW_ROOM_BRANCH;
    }
  }

  encounter_remaining = request->encounter_budget;
  hazard_remaining = request->hazard_budget;
  for (i = 1; i < count; ++i) {
    aw_instance_room *room = &temp.rooms[i];
    if (i == count - 1) {
      uint32_t cost = encounter_remaining < 3 ? encounter_remaining : 3;
      room->encounter_cost = (uint8_t)cost;
      encounter_remaining -= cost;
    } else if (encounter_remaining > 0 && (aw_instance_next_random(&rng) & 1U)) {
      room->encounter_cost = 1;
      --encounter_remaining;
    }
    if (hazard_remaining > 0 && i + 1 < count &&
        aw_instance_next_random(&rng) % 3 == 0) {
      room->hazard_cost = 1;
      --hazard_remaining;
    }
  }

  if (aw_instance_validate(&temp) != AW_INSTANCE_OK)
    return AW_INSTANCE_ERR_GENERATION;
  temp.state = AW_INSTANCE_VALIDATED;
  *out = temp;
  return AW_INSTANCE_OK;
}

aw_instance_error aw_instance_transition(aw_instance *instance,
                                         aw_instance_state next_state) {
  aw_instance_state current;
  if (instance == NULL) return AW_INSTANCE_ERR_ARGUMENT;
  current = instance->state;
  if ((current == AW_INSTANCE_CREATED && (next_state == AW_INSTANCE_VALIDATED ||
                                           next_state == AW_INSTANCE_REJECTED)) ||
      (current == AW_INSTANCE_VALIDATED && (next_state == AW_INSTANCE_ACTIVE ||
                                             next_state == AW_INSTANCE_REJECTED)) ||
      (current == AW_INSTANCE_ACTIVE && (next_state == AW_INSTANCE_COMPLETED ||
                                          next_state == AW_INSTANCE_FAILED ||
                                          next_state == AW_INSTANCE_ABANDONED)) ||
      ((current == AW_INSTANCE_COMPLETED || current == AW_INSTANCE_FAILED ||
        current == AW_INSTANCE_ABANDONED || current == AW_INSTANCE_REJECTED) &&
       next_state == AW_INSTANCE_ARCHIVED)) {
    instance->state = next_state;
    return AW_INSTANCE_OK;
  }
  return AW_INSTANCE_ERR_STATE;
}

aw_instance_error aw_tower_run_init(aw_tower_run *run, uint64_t run_id,
                                    uint32_t world_seed, uint16_t max_floor) {
  if (run == NULL || run_id == 0 || world_seed == 0) return AW_INSTANCE_ERR_ARGUMENT;
  if (max_floor == 0 || max_floor > AW_TOWER_MAX_FLOORS) return AW_INSTANCE_ERR_RANGE;
  memset(run, 0, sizeof(*run));
  run->run_id = run_id;
  run->world_seed = world_seed;
  run->current_floor = 1;
  run->max_floor = max_floor;
  run->state = AW_INSTANCE_ACTIVE;
  return AW_INSTANCE_OK;
}

aw_instance_error aw_tower_complete_floor(aw_tower_run *run,
                                          uint64_t reward_transaction_id) {
  if (run == NULL || reward_transaction_id == 0) return AW_INSTANCE_ERR_ARGUMENT;
  if (run->state != AW_INSTANCE_ACTIVE) return AW_INSTANCE_ERR_STATE;
  if (reward_transaction_id == run->last_reward_transaction_id)
    return AW_INSTANCE_ERR_STATE;
  run->last_reward_transaction_id = reward_transaction_id;
  if (run->current_floor >= run->max_floor) {
    run->state = AW_INSTANCE_COMPLETED;
    return AW_INSTANCE_OK;
  }
  ++run->current_floor;
  return AW_INSTANCE_OK;
}

aw_instance_error aw_tower_fail(aw_tower_run *run) {
  if (run == NULL) return AW_INSTANCE_ERR_ARGUMENT;
  if (run->state != AW_INSTANCE_ACTIVE) return AW_INSTANCE_ERR_STATE;
  run->state = AW_INSTANCE_FAILED;
  return AW_INSTANCE_OK;
}

aw_instance_error aw_tower_abandon(aw_tower_run *run) {
  if (run == NULL) return AW_INSTANCE_ERR_ARGUMENT;
  if (run->state != AW_INSTANCE_ACTIVE) return AW_INSTANCE_ERR_STATE;
  run->state = AW_INSTANCE_ABANDONED;
  return AW_INSTANCE_OK;
}
