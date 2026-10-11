#include "types.h"
#include "game/screen_other_map.h"
#include "game/sprite_anim.h"
#include "game/Unk_ov005_0236bbac.h"

s32 HwDiv32(s32 numer, s32 denom);
int MainLoop_GetFrameCount(void);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

extern s16 gSineTable[];

#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* scale moved toward 0x100 by (8 - n) / 8 of the way */
#define PULL(scale, n) ((s16)((((scale) - 0x100) * (n) >> 3) + 0x100))

/* Per-frame sprite update of the Sound Room. Sets OBJ affine matrices 1 and
 * 2 on both screens to scale by objScale1 and objScale2; on frames 6 to 15
 * of every 16 the scale is pulled toward 1x and back, so the sprites pulse.
 * Then places the screen's objects. gSineTable has 256 steps per turn
 * (index 64 = cosine of 0), so the matrices have no rotation. */
void Ov005_UpdateSoundRoomSprites(Unk_ov005_0236bbac *work)
{
    s16 scale1 = work->objScale1;
    s16 scale2;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    /* needed to match: u32 (int swaps registers) */
    u32 cosValue;
    u32 negSin;
    u32 sinValue;

    if (scale1 != -1 || work->objScale2 != -1) {
        scale2 = work->objScale2;
        switch (MainLoop_GetFrameCount() & 15) {
        case 6:
        case 15:
            scale1 = PULL(scale1, 7);
            scale2 = PULL(scale2, 7);
            break;
        case 7:
        case 14:
            scale1 = PULL(scale1, 6);
            scale2 = PULL(scale2, 6);
            break;
        case 8:
        case 13:
            scale1 = PULL(scale1, 5);
            scale2 = PULL(scale2, 5);
            break;
        case 9:
        case 12:
            scale1 = PULL(scale1, 4);
            scale2 = PULL(scale2, 4);
            break;
        case 10:
        case 11:
            scale1 = PULL(scale1, 3);
            scale2 = PULL(scale2, 3);
            break;
        }
    } else {
        scale1 = work->objScaleBoth;
        if (scale1 != -1) {
            scale2 = scale1;
        } else {
            scale1 = 0x100;
            scale2 = scale1;
        }
    }

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    {
        int d3 = DIVISOR(scale1);
        int d2 = DIVISOR(scale1);
        int d1 = DIVISOR(scale1);
        int d0 = DIVISOR(scale1);

        cosValue = gSineTable[64] * 16;
        sinValue = gSineTable[0];
        negSin = -sinValue * 16;
        sinValue = sinValue * 16;
        Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(negSin, d1), HwDiv32(sinValue, d2),
                      HwDiv32(cosValue, d3));
    }
    {
        int d3 = DIVISOR(scale2);
        int d2 = DIVISOR(scale2);
        int d1 = DIVISOR(scale2);
        int d0 = DIVISOR(scale2);

        Oam_SetAffineParams(2, HwDiv32(cosValue, d0), HwDiv32(negSin, d1), HwDiv32(sinValue, d2),
                      HwDiv32(cosValue, d3));
    }
    work->subObjects[0]->tileBase = 0xc7a0;
    work->subObjects[1]->tileBase = 0xb380;
    work->subObjects[2]->tileBase = 0xb380;
    work->subObjects[3]->tileBase = 0xb380;
    work->subObjects[4]->tileBase = 0xb380;
    SpriteAnim_Draw(work->subObjects[0], 128, 176);
    SpriteAnim_Draw(work->subObjects[1], 16, 176);
    SpriteAnim_Draw(work->subObjects[2], 240, 176);
    SpriteAnim_Draw(work->subObjects[3], 512, 928);
    SpriteAnim_Draw(work->subObjects[4], 1248, 928);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    {
        int d3 = DIVISOR(scale1);
        int d2 = DIVISOR(scale1);
        int d1 = DIVISOR(scale1);
        int d0 = DIVISOR(scale1);

        Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(negSin, d1), HwDiv32(sinValue, d2),
                      HwDiv32(cosValue, d3));
    }
    {
        int d3 = DIVISOR(scale2);
        int d2 = DIVISOR(scale2);
        int d1 = DIVISOR(scale2);
        int d0 = DIVISOR(scale2);

        Oam_SetAffineParams(2, HwDiv32(cosValue, d0), HwDiv32(negSin, d1), HwDiv32(sinValue, d2),
                      HwDiv32(cosValue, d3));
    }
    work->mainObjects[0]->tileBase = 0xb380;
    work->mainObjects[1]->tileBase = 0xb380;
    work->mainObjects[2]->tileBase = 0xb380;
    work->mainObjects[3]->tileBase = 0xb380;
    SpriteAnim_Draw(work->mainObjects[0], 16, 16);
    SpriteAnim_Draw(work->mainObjects[1], 240, 16);
    SpriteAnim_Draw(work->mainObjects[2], 512, 768);
    SpriteAnim_Draw(work->mainObjects[3], 1248, 768);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
