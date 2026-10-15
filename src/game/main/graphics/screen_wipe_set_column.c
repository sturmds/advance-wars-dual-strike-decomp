#include "types.h"
#include "game/Unk_02088d04.h"

#define WIPE_FIRST_TILE \
  0x288 /* the wipe tiles unpack here (ScreenWipe_PhaseDownInit) */
#define WIPE_PHASE_MAX 6
#define MAP_ROW_ENTRIES 64 /* the layer's map is two screens wide */
#define WIPE_ROWS 12       /* 2x2 blocks down the screen */
#define TILE_FLIP_H 0x400

/* Fills one column of 2x2-tile blocks (WIPE_ROWS of them) on the first layer
 * with the wipe tiles of the given phase (0-6; out of range is clamped).
 * mirror 1 flips every block horizontally. */
void ScreenWipe_SetColumn(int column, int phase, int mirror) {
  u16* map;
  u16 tile;
  u8 row;

  if (phase >= WIPE_PHASE_MAX) {
    phase = WIPE_PHASE_MAX;
  }
  if (phase < 0) {
    phase = 0;
  }
  map = data_027c0280->layer0Map + column * 2;
  tile = WIPE_FIRST_TILE + phase * 4 + mirror * TILE_FLIP_H;
  for (row = 0; row < WIPE_ROWS; row++) {
    map[0] = tile + mirror;
    map[1] = tile + 1 - mirror;
    map[32] = tile + mirror + 2;
    map[33] = tile + 3 - mirror;
    map += MAP_ROW_ENTRIES;
  }
}
