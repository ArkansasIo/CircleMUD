#include "rpg_runtime_tick.h"

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
    initialized = 1;
  }

  rpg_runtime_tick(0.1);
  extract_pending_chars();
}
