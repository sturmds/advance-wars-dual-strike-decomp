#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
int MainLoop_GetFrameCount(void);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
int Math_SinDegrees(int degrees);

extern MenuState gMenuState;
extern s16 gSineTable[];
extern u8 data_027c0040;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* a scale of 0 would divide by zero */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Combat setup sub screen (link play): queues one icon per player in each
 * team row, spread evenly across the row. The local player's icon, while
 * the player is not ready (players[].ready == 0), uses OBJ affine matrix 30
 * at double size and pulses with the frame count; ready players are drawn
 * semi-transparent.
 * The first memberCount bytes of a row hold its players. */
void CombatSetup_DrawRowPlayerIcons(int x, int y)
{
    int scale;
    CombatRowEntry *row;
    s8 pal;                            /* needed to match: s8, set first in one branch and last in the other */
    int j;
    int i;
    u8 step;
    u8 px;
    u16 sx;
    u16 sy;
    u8 player;

    scale = (Math_SinDegrees(HwMod32(MainLoop_GetFrameCount() << 4, 360)) >> 9) + 0x100;

    {
        int d3 = DIVISOR(scale);
        int d2 = DIVISOR(scale);
        int d1 = DIVISOR(scale);
        int d0 = DIVISOR(scale);
        s32 cosValue = gSineTable[64] * 16;
        s32 sinValue = gSineTable[0];

        Oam_SetAffineParams(30, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                            HwDiv32(cosValue, d3));
    }

    row = MENU->combat.rows;
    for (i = 0; i < 4; i++, row++) {
        if (row->memberCount == 0) {
            continue;
        }
        step = HwDiv32((24 - row->memberCount * 2) * 8, row->memberCount + 1);
        if (step & 1) {
            step++;
        }
        if (row->memberCount & 1) {
            px = 88 - (row->memberCount >> 1) * (step + 16);
        } else {
            px = 80 - ((row->memberCount >> 1) - 1) * (step + 16) - (step >> 1);
        }
        for (j = 0; j < row->memberCount; j++) {
            player = (&row->player)[j];
            if (data_0229fb30.unk_45a != player || gMenuState.combat.players[player].ready != 0) {
                pal = 0;
                sx = (x + px) & 0x1ff;
                sy = (y + 9 + i * 32) & 0xff;
            } else {
                /* attribute 1 bits 9-13: matrix 30; attribute 0: affine, double size */
                sx = ((x + px - 8) & 0x1ff) | 0x3c00;
                sy = (u8)(y + 1 + i * 32) | 0x300;
                pal = 1;
            }
            if (gMenuState.combat.players[player].ready != 0) {
                sy |= 0x400; /* semi-transparent */
            }
            Sprite_Queue(2, sx, sy, &data_027c0040, pal << 12, player * 4 + 240);
            px += step + 16;
        }
    }
}
