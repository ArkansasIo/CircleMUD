#ifndef CIRCLEMUD_RPG_CIRCLEMUD_ITEM_MAP_H
#define CIRCLEMUD_RPG_CIRCLEMUD_ITEM_MAP_H

#include "rpg_catalog.h"

/* Explicit legacy VNUM -> RPG catalog mapping. */
typedef struct {
  int circlemud_vnum;
  int rpg_catalog_index;
} rpg_circlemud_item_mapping;

/* Return the mapped RPG catalog index, or -1 when no explicit mapping exists. */
int rpg_circlemud_item_catalog_index(int circlemud_vnum);

/* Return the CircleMUD VNUM for an RPG catalog item, or -1 when unmapped. */
int rpg_circlemud_item_vnum(int rpg_catalog_index);

/* Validate and expose the built-in mapping table. */
int rpg_circlemud_item_mapping_count(void);
const rpg_circlemud_item_mapping *rpg_circlemud_item_mappings(void);

#endif
