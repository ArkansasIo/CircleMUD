#ifndef CIRCLEMUD_RPG_RUNTIME_TICK_H
#define CIRCLEMUD_RPG_RUNTIME_TICK_H

/*
 * Adapter boundary between CircleMUD's heartbeat and the RPG runtime.
 * The caller owns the authoritative elapsed-time measurement and passes
 * seconds since the previous tick.
 */
void rpg_runtime_tick_init(void);
void rpg_runtime_tick(double delta_seconds);

#endif
