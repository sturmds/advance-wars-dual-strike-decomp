#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_gen.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
/* Per tile, 30 a row. The random map steps mark tiles here; Ov006_ScatterMountains
 * marks new mountain seeds with 2. */

#define MARK_CLEAR 0
#define MARK_DONE 1
#define MARK_SEED 2

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Turns every tile marked as a seed into done and clears the marks of its
 * four neighbours. Returns TRUE if there was a seed. */
BOOL Ov006_SettleSeedTiles(void) {
  BOOL found = FALSE;
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (Ov006_gGenMarks[y][x] == MARK_SEED) {
        if (x - 1 >= 0) {
          Ov006_gGenMarks[y][x - 1] = MARK_CLEAR;
        }
        if (y - 1 >= 0) {
          Ov006_gGenMarks[y - 1][x] = MARK_CLEAR;
        }
        if (x + 1 < STATE->unk_1c4->mapWidth) {
          Ov006_gGenMarks[y][x + 1] = MARK_CLEAR;
        }
        if (y + 1 < STATE->unk_1c4->mapHeight) {
          Ov006_gGenMarks[y + 1][x] = MARK_CLEAR;
        }
        Ov006_gGenMarks[y][x] = MARK_DONE;
        found = TRUE;
      }
    }
  }
  return found != FALSE;
}
