#include "rpg_circlemud_item_map.h"

static const rpg_circlemud_item_mapping mappings[] = {
  {1000, 0},
  {1001, 1},
  {1002, 2},
  {1003, 3},
  {1004, 4},
  {1100, 150},
  {1101, 151},
  {1102, 152},
  {1200, 300},
  {1201, 301},
  {1300, 500},
  {1301, 501},
  {1400, 650},
  {1401, 651},
  {1500, 750}
};

int rpg_circlemud_item_mapping_count(void) {
  return (int)(sizeof(mappings) / sizeof(mappings[0]));
}

const rpg_circlemud_item_mapping *rpg_circlemud_item_mappings(void) {
  return mappings;
}

int rpg_circlemud_item_catalog_index(int circlemud_vnum) {
  int i;

  if (circlemud_vnum < 0)
    return -1;

  for (i = 0; i < rpg_circlemud_item_mapping_count(); ++i) {
    if (mappings[i].circlemud_vnum == circlemud_vnum &&
        rpg_catalog_valid_index(mappings[i].rpg_catalog_index))
      return mappings[i].rpg_catalog_index;
  }

  return -1;
}

int rpg_circlemud_item_vnum(int rpg_catalog_index) {
  int i;

  if (!rpg_catalog_valid_index(rpg_catalog_index))
    return -1;

  for (i = 0; i < rpg_circlemud_item_mapping_count(); ++i)
    if (mappings[i].rpg_catalog_index == rpg_catalog_index)
      return mappings[i].circlemud_vnum;

  return -1;
}
