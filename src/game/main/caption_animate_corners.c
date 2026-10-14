#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int CaptionFont_TextWidth(const u8* str);
int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(void* task);

/* Draws the caption centered at y 65 with two corner sprites, top right
 * and bottom left, that step through five cells over 14 frames. The
 * script resumes after frame 14. */
void Caption_AnimateCorners(CaptionTask* task) {
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
  x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
    x +=
        CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff, 65, 0, 0, 1);
  }
  left = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
  right = 128 + (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
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
  Sprite_QueueAt(0, (right - 10) & 0x1ff, 51, &data_027c0000, 0x4000,
                 cell * 8 + 0xc0);
  Sprite_QueueAt(0, (left - 7) & 0x1ff, 79, &data_027c0000, 0x4000,
                 cell * 8 + 0xc0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == 14) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
