#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

extern u8 data_027c0058[];

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int CaptionFont_TextWidth(const u8* str);
int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(void* task);

/* Draws the caption centered at y 65 with a pair of sprites above it that
 * slides right across its width over 15 frames, and a pair below it
 * whose x is right minus the first pair's. The script resumes after
 * frame 16. */
void Caption_SweepBars(CaptionTask* task) {
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
  left = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  right = 128 + (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  sweep = Math_Ease(0, left, right - 18, task->frame - 1, 15);
  Sprite_QueueAt(0, sweep & 0x1ff, 61, data_027c0058, 0x4000, 0xe8);
  Sprite_QueueAt(0, sweep & 0x1ff, 63, data_027c0058, 0x4000, 0xe8);
  Sprite_QueueAt(0, (right - sweep) & 0x1ff, 90, data_027c0058, 0x4000, 0xe8);
  Sprite_QueueAt(0, (right - sweep) & 0x1ff, 92, data_027c0058, 0x4000, 0xe8);
  x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
    x +=
        CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff, 65, 0, 0, 1);
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == 16) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
