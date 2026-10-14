#include "types.h"

#define EVENT_TEXT_COLUMNS 25 /* tiles of a text line */
#define BG_MAP_COLUMNS 32     /* tiles of a row of the map */

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
      for (col = 0; col < EVENT_TEXT_COLUMNS; col++) {
        (tile + offset)[col] = 0;
      }
      row++;
      offset += BG_MAP_COLUMNS;
    } while (row < total);
  }
}
