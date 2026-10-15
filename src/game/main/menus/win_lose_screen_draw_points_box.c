#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void WinLoseScreen_DrawNumber(int x, int y, int value);
void WinLoseScreen_DrawPoints(int x, int y, int value, int frame);

/* Draws the points box of the win/lose screen on the main screen, like
 * PointsTally_Draw: in Survival only the total (at y 153), otherwise the
 * points added (y 128) and the total (y 145), each between a label and a
 * suffix sprite. A negative total shows as 0. Only the total pops in;
 * popFrame -1 draws it at rest. */
void WinLoseScreen_DrawPointsBox(s32 added, s32 total, int popFrame) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (total < 0) {
    total = 0;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (data_027c027c->unk_48[data_027c0284]->gameMode == GAME_MODE_SURVIVAL) {
    Sprite_QueueAt(6, 26, 153, &data_027c0108, 0x60d8, 0);
    WinLoseScreen_DrawPoints(131, 153, total, popFrame);
    Sprite_QueueAt(6, 194, 153, &data_027c0060, 0x6120, 0);
  } else {
    Sprite_QueueAt(6, 65, 128, &data_027c0040, 0x60f4, 0);
    WinLoseScreen_DrawNumber(111, 128, added);
    Sprite_QueueAt(6, 178, 128, &data_027c0060, 0x6120, 0);
    Sprite_QueueAt(6, 42, 145, &data_027c0108, 0x60d8, 0);
    WinLoseScreen_DrawPoints(111, 145, total, popFrame);
    Sprite_QueueAt(6, 178, 145, &data_027c0060, 0x6120, 0);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
