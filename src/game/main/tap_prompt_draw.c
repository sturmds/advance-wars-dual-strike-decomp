#include "types.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"
#include "game/screen.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Draws the prompt: the sprite sits near the bottom right of the sub screen. */
void TapPrompt_Draw(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  TotalRankScreen_QueueTopCoSprites(0);
  SubScreen_QueueTwoSprites(0);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Sprite_QueueAt(3, 222, 144, &data_027c0060, 0x70b8, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
