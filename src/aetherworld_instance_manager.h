#ifndef AETHERWORLD_INSTANCE_MANAGER_H
#define AETHERWORLD_INSTANCE_MANAGER_H

#include <stddef.h>
#include <stdint.h>
#include "aetherworld_instance.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AW_INSTANCE_MANAGER_MAX_INSTANCES 64
#define AW_INSTANCE_MANAGER_MAX_PLAYERS 20

typedef struct aw_managed_instance {
  uint64_t instance_id;
  aw_instance instance;
  long player_ids[AW_INSTANCE_MANAGER_MAX_PLAYERS];
  uint16_t player_count;
  uint16_t max_players;
  uint8_t occupied;
} aw_managed_instance;

typedef struct aw_instance_manager {
  uint64_t next_instance_id;
  aw_managed_instance slots[AW_INSTANCE_MANAGER_MAX_INSTANCES];
} aw_instance_manager;

typedef enum aw_manager_error {
  AW_MANAGER_OK = 0,
  AW_MANAGER_ERR_ARGUMENT = 1,
  AW_MANAGER_ERR_FULL = 2,
  AW_MANAGER_ERR_NOT_FOUND = 3,
  AW_MANAGER_ERR_PLAYER = 4,
  AW_MANAGER_ERR_CAPACITY = 5,
  AW_MANAGER_ERR_STATE = 6,
  AW_MANAGER_ERR_GENERATION = 7
} aw_manager_error;

void aw_instance_manager_init(aw_instance_manager *manager);
aw_manager_error aw_instance_manager_create(aw_instance_manager *manager,
                                             const aw_instance_request *request,
                                             uint16_t max_players,
                                             uint64_t *instance_id);
aw_manager_error aw_instance_manager_find(aw_instance_manager *manager,
                                          uint64_t instance_id,
                                          aw_managed_instance **out);
aw_manager_error aw_instance_manager_start(aw_instance_manager *manager,
                                            uint64_t instance_id);
aw_manager_error aw_instance_manager_join(aw_instance_manager *manager,
                                           uint64_t instance_id, long player_id);
aw_manager_error aw_instance_manager_leave(aw_instance_manager *manager,
                                            uint64_t instance_id, long player_id);
aw_manager_error aw_instance_manager_complete(aw_instance_manager *manager,
                                               uint64_t instance_id);
aw_manager_error aw_instance_manager_fail(aw_instance_manager *manager,
                                           uint64_t instance_id);
aw_manager_error aw_instance_manager_destroy(aw_instance_manager *manager,
                                              uint64_t instance_id);
size_t aw_instance_manager_active_count(const aw_instance_manager *manager);
int aw_instance_manager_has_player(const aw_managed_instance *instance,
                                   long player_id);

#ifdef __cplusplus
}
#endif
#endif
