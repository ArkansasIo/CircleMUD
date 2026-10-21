#include <assert.h>
#include <stdio.h>

#include "../src/rpg/rpg_circlemud_item_map.h"

int main(void) {
  const rpg_circlemud_item_mapping *mappings;
  int count;

  count = rpg_circlemud_item_mapping_count();
  assert(count > 0);

  mappings = rpg_circlemud_item_mappings();
  assert(mappings != NULL);
  assert(mappings[0].circlemud_vnum == 1000);
  assert(rpg_circlemud_item_catalog_index(1000) == 0);
  assert(rpg_circlemud_item_catalog_index(1100) == 150);
  assert(rpg_circlemud_item_catalog_index(1500) == 750);
  assert(rpg_circlemud_item_catalog_index(999) == -1);
  assert(rpg_circlemud_item_catalog_index(-1) == -1);

  printf("rpg CircleMUD item mapping tests passed\n");
  return 0;
}
