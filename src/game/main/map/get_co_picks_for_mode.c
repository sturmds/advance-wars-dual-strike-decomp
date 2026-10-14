#include "types.h"

/* CO picks kept between maps, [player 1-4][front, back] (row 0 unused),
 * one table per mode. Player setup (Map_SetupPlayerCos) reads and refills them. */
extern u8 gCoPicksMode0[][2];
extern u8 gCoPicksMode1[][2];
extern u8 gCoPicksMode2[][2];
extern u8 gCoPicksMode3[][2];

/* mode: Unk_02080440_48.gameMode. */
u8 (*GetCoPicksForMode(int mode))[2] {
  switch (mode) {
    case 0:
      return gCoPicksMode0;
    case 1:
      return gCoPicksMode1;
    case 3:
      return gCoPicksMode3;
    case 2: /* needed to match: keeps 2 in the jump table */
    default:
      return gCoPicksMode2;
  }
}
