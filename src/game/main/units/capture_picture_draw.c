#include "types.h"
#include "game/Unk_02022404.h"
#include "game/capture_picture_task.h"

extern s16 gSineTable[];

s32 HwDiv32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Oam_PutSmallNumber(u16 x, u16 y, int value);

/* Frame step of the capture picture task: places the three sprites at
 * (x, y) and squashes affine matrix 3 vertically to (20 - points) / 20, so
 * the sprite in unk_38 shrinks and moves down as the capture goes on while
 * the one in unk_34 moves down 2 pixels a point. The number above shows
 * the points still needed. gSineTable has 256 steps per turn (index 64:
 * cosine of 0). */
void CapturePicture_Draw(CapturePictureTask *task)
{
    s16 x = task->x & 0x1ff;
    s16 y = task->y & 0xff;
    s16 scale = HwDiv32((20 - task->points) << 8, 20);
    /* how far the squashed sprite's top moves down */
    s16 drop = HwDiv32((256 - scale) << 4, 256);
    s16 unk34Y;
    int top = task->y - 16 + task->unk_42;
    int d1;
    int d0;
    int sinValue;
    int cosValue;

    /* needed to match: assigned here, not in the declaration */
    unk34Y = (top + (s16)(-(20 - task->points) * 2)) & 0xff;
    SpriteAnim_Draw(task->picture, x | 0x200, y | 0x100);
    SpriteAnim_Draw(task->unk_38, x | 0x600, (s16)((top + drop) & 0xff) | 0x100);
    SpriteAnim_Draw(task->unk_34, x | 0x400, unk34Y | 0x100);
    d1 = scale ? scale : 2;
    d0 = scale ? scale : 2;
    cosValue = gSineTable[64] * 16;
    sinValue = gSineTable[0];
    Oam_SetAffineParams(3, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, d0),
                        HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, d1));
    Oam_PutSmallNumber(x, y - 12, 20 - task->points);
}
