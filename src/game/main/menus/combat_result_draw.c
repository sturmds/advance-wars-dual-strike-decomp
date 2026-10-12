#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int func_02039f20(const u8* str);
int Text_GetStringWidth(const u8* str);
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
int CaptionLetter_GetAdvance(int ch, u16 x, u16 y, int attr, u16 firstTile,
                             u8 list);
void func_0200270c(int x, int y, int attr, int list, int hFlip);

/* Per-frame update of the Combat result task (data_0216168c): queues the
 * sprite of func_0200270c at (192, 160) on the screen of data_027c0288.unk_00
 * (the CO that CombatResult_Init loads, a guess), and the two caption lines
 * centered on the other screen, as TwoLineCaption_Draw does but with
 * attributes 0 and sprite list 0. */
void CombatResult_Draw(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_0200270c(192, 160, 0, 8, 0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  x = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_A)); i++) {
    x += SpriteFont_QueueChar(Text_Get(TWO_LINE_CAPTION_TEXT_A)[i], x & 0x1ff,
                              80, 0, 0, 0);
  }
  x = 128 - (Text_GetStringWidth(Text_Get(TWO_LINE_CAPTION_TEXT_B)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_B)); i++) {
    x += CaptionLetter_GetAdvance(Text_Get(TWO_LINE_CAPTION_TEXT_B)[i],
                                  x & 0x1ff, 72, 0, 0, 0);
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
