#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int func_02039f20(const u8* str);
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile,
                         u8 list);
void Task_ResumeScript(CaptionTask* task);

/* Needed to match: const lets mwcc load the sine and cosine once, before
 * the loop. */
extern const s16 gSineTable[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Draws caption line A on the screen of data_027c0288.unk_00, one letter
 * more every 6 frames. Each letter grows in over 16 frames through its
 * own affine matrix (letter i uses matrix i + 1): its width eases from
 * 1/128 to full, its height from 2x to 1x. The script resumes once the last
 * letter is at rest. */
void TwoLineCaption_GrowInLetters(CaptionTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  int x;
  int scaleX;
  int scaleY;

  task->frame++;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  x = 128 - (func_02039f20(Text_Get(TWO_LINE_CAPTION_TEXT_A)) >> 1);
  for (i = 0; i < strlen(Text_Get(TWO_LINE_CAPTION_TEXT_A)); i++) {
    if (task->frame < i * 6) {
      continue;
    }
    if (task->frame < i * 6 + 16) {
      scaleX = Math_Ease(0, 2, 0x100, task->frame - i * 6, 16);
      scaleY = Math_Ease(0, 0x200, 0x100, task->frame - i * 6, 16);
      {
        int dy2 = DIVISOR(scaleY);
        int dx2 = DIVISOR(scaleX);
        int dy = DIVISOR(scaleY);
        int dx = DIVISOR(scaleX);

        Oam_SetAffineParams(i + 1, HwDiv32(gSineTable[64] * 16, dx),
                            HwDiv32(-gSineTable[0] * 16, dy),
                            HwDiv32(gSineTable[0] * 16, dx2),
                            HwDiv32(gSineTable[64] * 16, dy2));
      }
      x += SpriteFont_QueueChar(Text_Get(TWO_LINE_CAPTION_TEXT_A)[i],
                                (x & 0x1ff) | ((i + 1) << 9), 80 | 0x300,
                                0x1000, 0, 6);
    } else {
      x += SpriteFont_QueueChar(Text_Get(TWO_LINE_CAPTION_TEXT_A)[i], x & 0x1ff,
                                80, 0x1000, 0, 6);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame == strlen(Text_Get(TWO_LINE_CAPTION_TEXT_A)) * 6 + 16) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
