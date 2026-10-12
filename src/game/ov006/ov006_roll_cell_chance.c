#include "types.h"

s32 Rng_Range(s32 min, s32 max);

/* Map cell codes, with the terrain Ov006_SetMapCell gives them. */
#define CELL_SHOAL 0
#define CELL_MOUNTAIN 32
#define CELL_SEA 42

/* Random map step: rolls whether a tile becomes `cell`, with better odds the
 * higher count is (the caller passes how many nearby tiles favour it).
 * Mountain: count 1-4 gives 2, 3, 3, 5 in 8. Sea: 2, 4, 5, 6 in 6. Shoal:
 * count 0-2 gives 1, 3, 4 in 4. Count 0 never gives mountain or sea; a count
 * past the table keeps its own value as the odds. Other cells never pass. */
BOOL Ov006_RollCellChance(int count, int cell) {
  int roll;

  switch (cell) {
    case CELL_MOUNTAIN:
      if (count == 0) {
        break;
      }
      roll = Rng_Range(1, 8);
      switch (count) {
        case 1:
          count = 2;
          break;
        case 2:
          count = 3;
          break;
        case 3:
          count = 3;
          break;
        case 4:
          count = 5;
          break;
      }
      if (roll <= count) {
        return TRUE;
      }
      break;
    case CELL_SEA:
      if (count == 0) {
        break;
      }
      roll = Rng_Range(1, 6);
      switch (count) {
        case 1:
          count = 2;
          break;
        case 2:
          count = 4;
          break;
        case 3:
          count = 5;
          break;
        case 4:
          count = 6;
          break;
      }
      if (roll <= count) {
        return TRUE;
      }
      break;
    case CELL_SHOAL:
      roll = Rng_Range(1, 4);
      switch (count) {
        case 0:
          count = 1;
          break;
        case 1:
          count = 3;
          break;
        case 2:
          count = 4;
          break;
      }
      if (roll <= count) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
