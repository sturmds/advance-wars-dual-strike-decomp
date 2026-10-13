#ifndef GAME_UNIT_TARGET_ENTRY_H
#define GAME_UNIT_TARGET_ENTRY_H

#include "types.h"

/* 8-byte entry of the AI target list (data_022a6dcc); data_022a6dd4 counts them. */
typedef struct UnitTargetEntry {
  u16 unitSlot;
  u16 value; /* object kind for a map object; for a unit 0 in the attack
                                        * target list, its damage in the map object fire list */
  s16 x;     /* tile */
  s16 y;
} UnitTargetEntry;

extern int
    gFireTargetCount; /* entries in the fire target list (FireTargets_Add) */

#endif
