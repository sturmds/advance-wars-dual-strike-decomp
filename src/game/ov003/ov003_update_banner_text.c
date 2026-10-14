#include "types.h"
#include "game/ov003_gfx.h"
#include "game/screen_other_map.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_0235dd38.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Mem_Copy32(const void* src, void* dst, u32 size);
const u8* Text_Get(u32 textId);
void Task_ResumeScript(void* task);
int strlen(const u8* str); /* string length */
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc,
                         s16 pd); /* sets OBJ affine matrix index */
int Ov003_DrawLetterAlt(int ch, u16 attr1,
                        u16 attr0); /* queues one letter, returns its width */
int Ov003_GetTextWidth(const u8* str); /* text width in pixels */

/* Sine table: [0] sin 0, [64] cos 0. Needed to match: const, so the
 * loads move out of the loops. */
extern const s16 gSineTable[];

#define STATE data_ov003_02364928

/* Needed to match: the original reads the frame counter as a signed
 * halfword. */
#define FRAME (*(s16*)&task->unk_4c)

#define TEXT_BANNER_TOP 0x7d000021
#define TEXT_BANNER_BOTTOM 0x7d000022

#define LETTER_DELAY 6          /* frames between two letters */
#define LETTER_FRAMES 16        /* frames a letter takes to grow in */
#define OAM_AFFINE_DOUBLE 0x300 /* OAM attribute 0: affine, double size */

#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Letter n of a line, while it grows in: width from 2/256 to full, height
 * from double to full, through OBJ affine matrix n. */
#define DRAW_GROWING_LETTER(text, ch, n, y)                                   \
  do {                                                                        \
    scaleX = Math_Ease(0, 2, 256, FRAME - (n) * LETTER_DELAY, LETTER_FRAMES); \
    scaleY =                                                                  \
        Math_Ease(0, 512, 256, FRAME - (n) * LETTER_DELAY, LETTER_FRAMES);    \
    {                                                                         \
      int d3 = DIVISOR(scaleY);                                               \
      int d2 = DIVISOR(scaleX);                                               \
      int d1 = DIVISOR(scaleY);                                               \
      int d0 = DIVISOR(scaleX);                                               \
      Oam_SetAffineParams((n), HwDiv32(gSineTable[64] * 16, d0),              \
                          HwDiv32(-gSineTable[0] * 16, d1),                   \
                          HwDiv32(gSineTable[0] * 16, d2),                    \
                          HwDiv32(gSineTable[64] * 16, d3));                  \
    }                                                                         \
    x += Ov003_DrawLetterAlt(Text_Get(text)[ch], (x & 0x1ff) | ((n) << 9),    \
                             OAM_AFFINE_DOUBLE | (y));                        \
  } while (0)

/* Task update: shows two centred lines of text, top at y 64 and bottom at
 * y 96, one letter every LETTER_DELAY frames. Each letter grows in over
 * LETTER_FRAMES frames. The frame counter stops while Ov003State.unk_1d7
 * is set. Frame 1 copies 0x2000 bytes from data_ov003_02365b0c into
 * OBJ VRAM. When the last letter is done, the task's script goes on. */
void Ov003_UpdateBannerText(Unk_ov003_0235dd38* task) {
  int top;
  int x;
  int bottom;
  u16 scaleX;
  u16 scaleY;

  if (STATE->unk_1d7 == 0) {
    FRAME++;
  }
  if (FRAME == 1) {
    Mem_Copy32(data_ov003_02365b0c, data_027c0280->objVram + 0xb1c0,
               OV003_TEXT_TILES_SIZE);
  }

  x = 128 - (Ov003_GetTextWidth(Text_Get(TEXT_BANNER_TOP)) >> 1);
  for (top = 0; top < strlen(Text_Get(TEXT_BANNER_TOP)); top++) {
    if (FRAME < top * LETTER_DELAY) {
      continue;
    }
    if (FRAME < top * LETTER_DELAY + LETTER_FRAMES) {
      DRAW_GROWING_LETTER(TEXT_BANNER_TOP, top, top, 64);
    } else {
      x += Ov003_DrawLetterAlt(Text_Get(TEXT_BANNER_TOP)[top], x & 0x1ff, 64);
    }
  }

  /* the bottom line's letters come after the top line's */
  x = 128 - (Ov003_GetTextWidth(Text_Get(TEXT_BANNER_BOTTOM)) >> 1);
  for (bottom = 0; bottom < strlen(Text_Get(TEXT_BANNER_BOTTOM)); bottom++) {
    if (FRAME < (top + bottom) * LETTER_DELAY) {
      continue;
    }
    if (FRAME < (top + bottom) * LETTER_DELAY + LETTER_FRAMES) {
      DRAW_GROWING_LETTER(TEXT_BANNER_BOTTOM, bottom, top + bottom, 96);
    } else {
      x += Ov003_DrawLetterAlt(Text_Get(TEXT_BANNER_BOTTOM)[bottom], x & 0x1ff,
                               96);
    }
  }

  if (FRAME == (top + bottom) * LETTER_DELAY + LETTER_FRAMES) {
    task->unk_4c = 0;
    Task_ResumeScript(task);
  }
}
