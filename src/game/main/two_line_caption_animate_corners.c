#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

extern u8 data_027c0000[];

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int func_02039f20(const u8* str);
int Text_GetStringWidth(const u8* str);
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
int CaptionLetter_GetAdvance(int ch, u16 x, u16 y, int attr, u16 firstTile,
                             u8 list);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CaptionTask* task);

/* Draws both caption lines and two corner sprites around line A, top
 * right and bottom left, that step through five cells over 14 frames.
 * The script resumes after frame 14. */
void TwoLineCaption_AnimateCorners(CaptionTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;
  int left;
  int right;
  int cell;

  task->frame++;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  x = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_A)); i++) {
    x += SpriteFont_QueueChar(Text_Get(TWO_LINE_CAPTION_TEXT_A)[i], x & 0x1ff,
                              80, 0x1000, 0, 6);
  }
  x = 128 - (Text_GetStringWidth(Text_Get(TWO_LINE_CAPTION_TEXT_B)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_B)); i++) {
    x += CaptionLetter_GetAdvance(Text_Get(TWO_LINE_CAPTION_TEXT_B)[i],
                                  x & 0x1ff, 72, 0x1000, 0, 6);
  }
  left = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  right = 128 + (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  if (task->frame <= 3) {
    cell = 0;
  } else if (task->frame <= 7) {
    cell = 1;
  } else if (task->frame <= 10) {
    cell = 2;
  } else if (task->frame <= 12) {
    cell = 3;
  } else {
    cell = 4;
  }
  Sprite_QueueAt(6, (right - 10) & 0x1ff, 66, data_027c0000, 0x1000,
                 cell * 8 + 0x30a);
  Sprite_QueueAt(6, (left - 7) & 0x1ff, 94, data_027c0000, 0x1000,
                 cell * 8 + 0x30a);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == 14) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
