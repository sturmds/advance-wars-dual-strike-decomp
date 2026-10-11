#include "types.h"
#include "game/caption_task.h"
#include "game/screen_other_map.h"

const u8 *Text_Get(u32 textId);
int strlen(const u8 *str);
int CaptionFont_TextWidth(const u8 *str);
int CaptionFont_DrawChar(int ch, u16 x, u16 y, int attr, u16 firstTile, u8 list);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int HwDiv32(int num, int den);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(void *task);

/* Sine table, index 64 = cosine of 0. Needed to match: const lets mwcc
 * load the sine and cosine once, before the loop. */
extern const s16 gSineTable[];

#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

/* Divides v by the eased scale s, or by 2 if s is 0. */
#define DIV_SCALE(v, s) HwDiv32(v, (s) != 0 ? (s) : 2)

/* Draws the caption at y 80 on the screen of data_027c0288.unk_00, one
 * letter more every 6 frames. Each letter grows in over 16 frames through
 * its own affine matrix (letter i uses matrix i + 1): its width eases from
 * 1/128 to full, its height from 2x to 1x. The script resumes once the
 * last letter is at rest. */
void Caption_GrowInLetters(CaptionTask *task)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    int i;
    int x;
    int scaleX;
    int scaleY;

    task->frame++;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    x = 128 - (CaptionFont_TextWidth(Text_Get(CAPTION_TEXT)) >> 1);
    for (i = 0; i < strlen(Text_Get(CAPTION_TEXT)); i++) {
        if (task->frame >= i * 6) {
            if (task->frame < i * 6 + 16) {
                scaleX = Math_Ease(0, 2, 256, task->frame - i * 6, 16);
                scaleY = Math_Ease(0, 512, 256, task->frame - i * 6, 16);
                Oam_SetAffineParams(i + 1, DIV_SCALE(COS0 << 4, scaleX), DIV_SCALE(-SIN0 << 4, scaleY),
                                    DIV_SCALE(SIN0 << 4, scaleX), DIV_SCALE(COS0 << 4, scaleY));
                x += CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], (x & 0x1ff) | ((i + 1) << 9), 80 | 0x300, 0, 0, 1);
            } else {
                x += CaptionFont_DrawChar(Text_Get(CAPTION_TEXT)[i], x & 0x1ff, 80, 0, 0, 1);
            }
        }
    }
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    if (task->frame == strlen(Text_Get(CAPTION_TEXT)) * 6 + 16) {
        task->frame = 0;
        Task_ResumeScript(task);
    }
}
