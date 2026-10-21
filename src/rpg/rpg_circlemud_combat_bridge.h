#ifndef CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_BRIDGE_H
#define CIRCLEMUD_RPG_CIRCLEMUD_COMBAT_BRIDGE_H

#define RPG_CIRCLEMUD_EVENT_QUEUE 256

typedef enum {
  RPG_CIRCLEMUD_EVENT_DAMAGE = 1,
  RPG_CIRCLEMUD_EVENT_DEATH = 2,
  RPG_CIRCLEMUD_EVENT_XP = 3,
  RPG_CIRCLEMUD_EVENT_LOOT = 4
} rpg_circlemud_event_type;

typedef struct {
  rpg_circlemud_event_type type;
  int player_id;
  int monster_id;
  int source_key;
  double amount;
  int catalog_index;
  unsigned long sequence;
} rpg_circlemud_combat_event;

void rpg_circlemud_combat_bridge_init(void);
int rpg_circlemud_queue_damage(int player_id, int monster_id, int source_key, double amount);
int rpg_circlemud_queue_death(int player_id, int monster_id, int source_key);
int rpg_circlemud_queue_xp(int player_id, int monster_id, double amount);
int rpg_circlemud_queue_loot(int player_id, int monster_id, int catalog_index);
int rpg_circlemud_combat_event_count(void);
int rpg_circlemud_combat_event_get(int index, rpg_circlemud_combat_event *event);
void rpg_circlemud_combat_bridge_clear(void);

/* Legacy mutation requires an explicit host-side authorization check. */
int rpg_circlemud_combat_event_authorized(const rpg_circlemud_combat_event *event);

#endif
