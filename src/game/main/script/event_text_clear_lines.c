#include "types.h"

/* Blanks the text of lines lines of the event text window: 2 tile rows of 25
 * tiles each, from tile in a 32-tile-wide map. The do-while shape is needed
 * to match. */
void EventText_ClearLines(u16* tile, int lines) {
  int col;
  int row;
  int offset;
  int total = lines * 2;

  row = 0;
  if (total > 0) {
    offset = 0;
    do {
      for (col = 0; col < 25; col++) {
        (tile + offset)[col] = 0;
      }
      row++;
      offset += 32;
    } while (row < total);
  }
}
