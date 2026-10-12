#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int CaptionFont_TextWidth(const u8* str);
int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);
int func_0204cd00(void);

/* Moves the caption from y 80 to y -127 over 24 frames. It is drawn on
 * screen data_027c0288.unk_00 while still on it (y > -16), and on
 * data_027c0288.unk_04 at y + 192 once it reaches that screen. After frame
 * 24 it resumes the tasks of script data_0215b22c and its own script. */
void Caption_SlideUp(CaptionTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;
  int y;

  task->frame++;
  y = Math_Ease(4, 80, -127, task->frame, 24);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (y > -16) {
    x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
    for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
      x += CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff, y & 0xff,
                                0, 0, 1);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (y + 192 < 192) {
    x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
    for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
      x += CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff,
                                (y + 192) & 0xff, 0, 0, 1);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == 24) {
    task->frame = 0;
    func_0204cd00();
    Task_ResumeScript(task);
  }
}
