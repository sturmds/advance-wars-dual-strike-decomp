#ifndef GAME_RANGE_SPREAD_H
#define GAME_RANGE_SPREAD_H

#include "types.h"

#define RANGE_SPREAD_MAX_DOTS 204 /* dots the 0x660-byte buffer holds */
#define RANGE_SPREAD_FRAMES 8

/* One dot of the range spread task: it starts on the acting unit's
 * destination tile and reaches one tile of rangeMap after
 * RANGE_SPREAD_FRAMES frames. Positions are in pixels. The task keeps an
 * array of these in Task.unk_4c and the count in Task.unk_64. */
typedef struct RangeSpreadDot {
  s16 x;
  s16 y;
  s16 vx; /* pixels per frame */
  s16 vy;
} RangeSpreadDot;

/* The dot count, kept in Task.unk_64. */
#define RANGE_SPREAD_COUNT(task) ((task)->unk_64)

#endif
