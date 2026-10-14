#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int func_02039f20(const u8* str);
int Text_GetStringWidth(const u8* str);
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
int CaptionLetter_GetAdvance(int ch, u16 x, u16 y, int attr, u16 firstTile,
                             u8 list);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CaptionTask* task);

/* Draws both caption lines with a pair of sprites above line A that
 * slides right across its width over 15 frames, and a pair below it
 * whose x is right minus the first pair's. The script resumes after
 * frame 16. */
void TwoLineCaption_SweepBars(CaptionTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;
  int left;
  int right;
  int sweep;

  task->frame++;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  left = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  right = 128 + (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  sweep = Math_Ease(0, left, right - 18, task->frame - 1, 15);
  Sprite_QueueAt(6, sweep & 0x1ff, 76, &data_027c0058, 0x1000, 0x332);
  Sprite_QueueAt(6, sweep & 0x1ff, 78, &data_027c0058, 0x1000, 0x332);
  Sprite_QueueAt(6, (right - sweep) & 0x1ff, 105, &data_027c0058, 0x1000,
                 0x332);
  Sprite_QueueAt(6, (right - sweep) & 0x1ff, 107, &data_027c0058, 0x1000,
                 0x332);
  x = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_A)); i++) {
    x += SpriteFont_QueueChar(Text_Get(TWO_LINE_CAPTION_TEXT_A)[i], x & 0x1ff,
                              80, 0x1000, 0, 7);
  }
  x = 128 - (Text_GetStringWidth(Text_Get(TWO_LINE_CAPTION_TEXT_B)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_B)); i++) {
    x += CaptionLetter_GetAdvance(Text_Get(TWO_LINE_CAPTION_TEXT_B)[i],
                                  x & 0x1ff, 72, 0x1000, 0, 7);
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == 16) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
