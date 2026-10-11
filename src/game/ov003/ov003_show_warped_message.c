#include "types.h"
#include "game/ov003_message_task.h"
#include "game/ov003_unit.h"

#define STATE_BYTE_1D7 (((u8 *)&data_ov003_02364928->unk_1d6)[1])

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
const u8 *Text_Get(u32 textId);
int strlen(const u8 *str);
int Ov003_GetTextWidth(const u8 *str);
int Ov003_DrawLetter(u8 ch, u16 x, u16 y);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(Ov003MessageTask *task);

extern s16 gSineTable[];

#define MESSAGE_TEXT 0x7d000024
#define MESSAGE_FRAMES 16
#define MESSAGE_MATRIX 31

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Shows the text 0x7d000024 centred at y 848 (sprite y 0x350) for
 * MESSAGE_FRAMES frames, drawn through affine matrix 31. Over those frames
 * the matrix is set to scales that ease from 256 to 512 (x) and from 256 to
 * 2 (y), each as the divisor of a table cosine or sine. Then it resumes the
 * task's script. Needed to match: the four divisors are evaluated in this
 * order before the table reads. */
void Ov003_ShowWarpedMessage(Ov003MessageTask *task)
{
    u16 scaleX;
    u16 scaleY;
    int d3;
    int d2;
    int d1;
    int d0;
    int sinValue;
    int cosValue;
    int i;
    int x;

    if (STATE_BYTE_1D7 == 0) {
        task->frames++;
    }
    scaleX = Math_Ease(0, 256, 512, task->frames, 16);
    scaleY = Math_Ease(0, 256, 2, task->frames, 16);
    d3 = DIVISOR(scaleY);
    d2 = DIVISOR(scaleX);
    d1 = DIVISOR(scaleY);
    d0 = DIVISOR(scaleX);
    cosValue = gSineTable[64] * 16;
    sinValue = gSineTable[0];
    Oam_SetAffineParams(MESSAGE_MATRIX, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                  HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
    x = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_TEXT)) >> 1);
    for (i = 0; i < strlen(Text_Get(MESSAGE_TEXT)); i++) {
        x += Ov003_DrawLetter(Text_Get(MESSAGE_TEXT)[i], (x & 0x1ff) | (MESSAGE_MATRIX << 9), 848);
    }
    if (task->frames == MESSAGE_FRAMES) {
        task->frames = 0;
        Task_ResumeScript(task);
    }
}
