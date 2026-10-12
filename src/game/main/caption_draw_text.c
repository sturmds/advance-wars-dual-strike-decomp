#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int CaptionFont_TextWidth(const u8* str);
int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);

/* Script step: draws the caption centered at y 65 on the screen of
 * data_027c0288.unk_04, without the corner sprites. */
void Caption_DrawText(CaptionTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
    x +=
        CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff, 65, 0, 0, 1);
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
