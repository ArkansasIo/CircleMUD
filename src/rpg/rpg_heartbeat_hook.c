#include "rpg_runtime_tick.h"
#include "rpg_circlemud_bridge.h"
#include "rpg_circlemud_mob_bridge.h"
#include "rpg_circlemud_host_adapter.h"
#include "rpg_circlemud_host_native.h"

/* Original CircleMUD declaration from handler.c. */
void extract_pending_chars(void);

/*
 * comm.c is compiled with extract_pending_chars() redirected to this hook.
 * CircleMUD calls it once from heartbeat(), so the RPG runtime advances once
 * for every authoritative heartbeat pulse, including catch-up pulses.
 */
void circlemud_rpg_extract_pending_chars(void) {
  static int initialized = 0;

  if (!initialized) {
    rpg_runtime_tick_init();
    rpg_circlemud_host_adapter_init();
    (void)rpg_circlemud_install_native_host_callbacks();
    initialized = 1;
  }

  /* Pull authoritative CircleMUD state into RPG-owned representations. */
  rpg_circlemud_sync_players();
  rpg_circlemud_sync_mob_prototypes();

  rpg_runtime_tick(0.1);
  extract_pending_chars();
}
