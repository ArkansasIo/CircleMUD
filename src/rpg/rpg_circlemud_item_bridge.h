#ifndef CIRCLEMUD_RPG_CIRCLEMUD_ITEM_BRIDGE_H
#define CIRCLEMUD_RPG_CIRCLEMUD_ITEM_BRIDGE_H

struct char_data;

/*
 * Copy CircleMUD object state into RPG-owned item storage.
 *
 * The bridge uses a deliberately narrow catalog policy: CircleMUD object VNUMs
 * in [0, RPG_CATALOG_TOTAL) map directly to the RPG catalog index. Other
 * VNUMs remain legacy-only until an explicit content mapping table is added.
 */
int rpg_circlemud_sync_items(struct char_data *ch);

#endif
