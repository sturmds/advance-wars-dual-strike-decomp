#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba8.h"
#include "game/icon.h"
#include "game/screen_other_map.h"

void Sprite_QueueDefault(int x, int y, int attr, int list);

/* On the second screen: queues a sprite at (40, 144), then draws an icon at
 * (8, 144) picked by the row of the selected CO grid cell. Rows 0-1 share
 * icon 75 and rows 5-6 share icon 79; the rows probably group COs by army. */
void Ov005_DrawCoGridRowIcon(Unk_ov005_0236bba8* work) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Sprite_QueueDefault(40, 144, 0xa1c0, 0);
  switch (work->grid[work->cursor].row) {
    case 0:
    case 1:
      Icon_Draw(75, 8, 144, 0, 0);
      break;
    case 2:
      Icon_Draw(76, 8, 144, 0, 0);
      break;
    case 3:
      Icon_Draw(77, 8, 144, 0, 0);
      break;
    case 4:
      Icon_Draw(78, 8, 144, 0, 0);
      break;
    case 5:
    case 6:
      Icon_Draw(79, 8, 144, 0, 0);
      break;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
