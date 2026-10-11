#include "types.h"
#include "game/title_call_task.h"
#include "game/screen_other_map.h"

int HwDiv32(int num, int den);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

extern const s16 gSineTable[];

/* Title call script step on the sub screen: resets the frame counter and
 * sets OBJ affine matrix 0 to the unscaled, unrotated matrix. */
void TitleCall_InitSubLetters(TitleCallTask *task)
{
    Unk_027c0280 *saved0280;
    int sinValue;
    int cosValue;

    task->frame = 0;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    cosValue = gSineTable[64];
    sinValue = gSineTable[0];
    Oam_SetAffineParams(0, HwDiv32(cosValue << 4, 256), HwDiv32(-sinValue << 4, 256), HwDiv32(sinValue << 4, 256),
                        HwDiv32(cosValue << 4, 256));
    Screen_SetCurrentGfx(saved0280);
}
