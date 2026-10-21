#include "aetherworld_instance_manager.h"

#include <string.h>

static aw_managed_instance *slot_for(aw_instance_manager *manager,
                                      uint64_t instance_id) {
  size_t i;
  if (manager == NULL || instance_id == 0) return NULL;
  for (i = 0; i < AW_INSTANCE_MANAGER_MAX_INSTANCES; ++i)
    if (manager->slots[i].occupied &&
        manager->slots[i].instance_id == instance_id)
      return &manager->slots[i];
  return NULL;
}

void aw_instance_manager_init(aw_instance_manager *manager) {
  if (manager == NULL) return;
  memset(manager, 0, sizeof(*manager));
  manager->next_instance_id = 1;
}

aw_manager_error aw_instance_manager_create(aw_instance_manager *manager,
                                             const aw_instance_request *request,
                                             uint16_t max_players,
                                             uint64_t *instance_id) {
  size_t i;
  aw_instance generated;
  if (manager == NULL || request == NULL || instance_id == NULL)
    return AW_MANAGER_ERR_ARGUMENT;
  if (max_players == 0 || max_players > AW_INSTANCE_MANAGER_MAX_PLAYERS)
    return AW_MANAGER_ERR_CAPACITY;
  if (aw_instance_generate(request, &generated) != AW_INSTANCE_OK)
    return AW_MANAGER_ERR_GENERATION;
  for (i = 0; i < AW_INSTANCE_MANAGER_MAX_INSTANCES; ++i) {
    aw_managed_instance *slot = &manager->slots[i];
    if (!slot->occupied) {
      memset(slot, 0, sizeof(*slot));
      slot->occupied = 1;
      slot->instance_id = manager->next_instance_id++;
      if (manager->next_instance_id == 0) manager->next_instance_id = 1;
      slot->instance = generated;
      slot->max_players = max_players;
      *instance_id = slot->instance_id;
      return AW_MANAGER_OK;
    }
  }
  return AW_MANAGER_ERR_FULL;
}

aw_manager_error aw_instance_manager_find(aw_instance_manager *manager,
                                          uint64_t instance_id,
                                          aw_managed_instance **out) {
  aw_managed_instance *slot;
  if (manager == NULL || out == NULL) return AW_MANAGER_ERR_ARGUMENT;
  slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  *out = slot;
  return AW_MANAGER_OK;
}

int aw_instance_manager_has_player(const aw_managed_instance *instance,
                                   long player_id) {
  uint16_t i;
  if (instance == NULL || player_id <= 0) return 0;
  for (i = 0; i < instance->player_count; ++i)
    if (instance->player_ids[i] == player_id) return 1;
  return 0;
}

aw_manager_error aw_instance_manager_start(aw_instance_manager *manager,
                                            uint64_t instance_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (aw_instance_transition(&slot->instance, AW_INSTANCE_ACTIVE) != AW_INSTANCE_OK)
    return AW_MANAGER_ERR_STATE;
  return AW_MANAGER_OK;
}

aw_manager_error aw_instance_manager_join(aw_instance_manager *manager,
                                           uint64_t instance_id, long player_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (player_id <= 0) return AW_MANAGER_ERR_PLAYER;
  if (slot->instance.state != AW_INSTANCE_ACTIVE)
    return AW_MANAGER_ERR_STATE;
  if (aw_instance_manager_has_player(slot, player_id))
    return AW_MANAGER_ERR_PLAYER;
  if (slot->player_count >= slot->max_players)
    return AW_MANAGER_ERR_CAPACITY;
  slot->player_ids[slot->player_count++] = player_id;
  return AW_MANAGER_OK;
}

aw_manager_error aw_instance_manager_leave(aw_instance_manager *manager,
                                            uint64_t instance_id, long player_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  uint16_t i;
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (player_id <= 0) return AW_MANAGER_ERR_PLAYER;
  for (i = 0; i < slot->player_count; ++i) {
    if (slot->player_ids[i] == player_id) {
      uint16_t last = (uint16_t)(slot->player_count - 1);
      slot->player_ids[i] = slot->player_ids[last];
      slot->player_ids[last] = 0;
      --slot->player_count;
      return AW_MANAGER_OK;
    }
  }
  return AW_MANAGER_ERR_PLAYER;
}

aw_manager_error aw_instance_manager_complete(aw_instance_manager *manager,
                                               uint64_t instance_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (aw_instance_transition(&slot->instance, AW_INSTANCE_COMPLETED) != AW_INSTANCE_OK)
    return AW_MANAGER_ERR_STATE;
  return AW_MANAGER_OK;
}

aw_manager_error aw_instance_manager_fail(aw_instance_manager *manager,
                                           uint64_t instance_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (aw_instance_transition(&slot->instance, AW_INSTANCE_FAILED) != AW_INSTANCE_OK)
    return AW_MANAGER_ERR_STATE;
  return AW_MANAGER_OK;
}

aw_manager_error aw_instance_manager_destroy(aw_instance_manager *manager,
                                              uint64_t instance_id) {
  aw_managed_instance *slot = slot_for(manager, instance_id);
  if (slot == NULL) return AW_MANAGER_ERR_NOT_FOUND;
  if (slot->player_count != 0) return AW_MANAGER_ERR_STATE;
  if (slot->instance.state != AW_INSTANCE_COMPLETED &&
      slot->instance.state != AW_INSTANCE_FAILED &&
      slot->instance.state != AW_INSTANCE_ABANDONED &&
      slot->instance.state != AW_INSTANCE_REJECTED)
    return AW_MANAGER_ERR_STATE;
  memset(slot, 0, sizeof(*slot));
  return AW_MANAGER_OK;
}

size_t aw_instance_manager_active_count(const aw_instance_manager *manager) {
  size_t i, count = 0;
  if (manager == NULL) return 0;
  for (i = 0; i < AW_INSTANCE_MANAGER_MAX_INSTANCES; ++i)
    if (manager->slots[i].occupied) ++count;
  return count;
}
