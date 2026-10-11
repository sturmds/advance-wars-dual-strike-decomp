#include "types.h"
#include "game/menu_state.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

#define REG_POLYGON_ATTR ((volatile u32 *)0x040004a4)
#define REG_TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define REG_PLTT_BASE ((volatile u32 *)0x040004ac)
#define REG_MTX_PUSH ((volatile u32 *)0x04000444)
#define REG_MTX_POP ((volatile u32 *)0x04000448)
#define REG_MTX_TRANS ((volatile u32 *)0x04000470)

extern MenuState gMenuState;
extern s16 data_0214d4c4[];

/* Needed to match: the side's offset into gMenuState is added at run time. SIDE_SWAY is the
 * same BattleSway as SIDE_STATE(side)->battle.sway[0], written another way so that the compiler
 * loads the base again after the call, as the original does. */
#define SIDE_STATE(side) ((MenuState *)((u8 *)&gMenuState + (side) * 8))
#define SIDE_SWAY(side) (((MenuState *)((BattleSway *)&gMenuState + (side)))->battle.sway[0])
#define SIDE_SCROLL(side) (((MenuState *)((u8 *)&gMenuState + (side) * 2))->battle.bgScroll[0])

void G3_RotX(int sin, int cos);
BOOL Battle_IsSideDrawnInAir(int side);
BOOL Battle_IsSideSea(int side);
int MainLoop_GetFrameCount(void);
void Gx_SendQuadXY(int x, int y, int z, int width, int height, int u0, int v0, int u1, int v1);
void Gx_SendQuadXZ(int x, int y, int z, int width, int depth, int u0, int v0, int u1, int v1);

/* Draws the ground of one battle side: an upright back panel, the floor, and for a Piperunner
 * (or type 30) its rail. The panel moves back by half the side's sway and turns back by the
 * sway angle. A sea floor scrolls with the frame count. When Battle_IsSideDrawnInAir says the side has
 * no ground, it draws nothing and only advances the side's BG scroll (bgScroll, which
 * Battle_DrawScene adds to the BG2 and BG3 scroll); a sea side advances it every second frame.
 * The two sides use their own textures and palettes. */
void Battle_DrawSideGround(int side)
{
    BOOL leftSide;
    int idx;
    int floorScroll;
    int transX;
    int transZ;

    if (Battle_IsSideDrawnInAir(side) == 1) {
        SIDE_SCROLL(side)++;
        return;
    }
    if (Battle_IsSideSea(side) == 1 && (MainLoop_GetFrameCount() & 1) == 0) {
        SIDE_SCROLL(side)++;
    }
    leftSide = side == 0 ? TRUE : FALSE;

    *REG_POLYGON_ATTR = 0x1f0081;      /* light 0, front faces, opaque */
    *REG_TEXIMAGE_PARAM = 0x6dc10000 | (((u32)(leftSide ? 0x200 : 0x280) << 5) >> 3);
    *REG_PLTT_BASE = ((u32)(leftSide ? 10 : 11) << 5) >> 4;
    *REG_MTX_PUSH = 0;
    idx = (-((SIDE_STATE(side)->battle.sway[0].angle + 0x600) & 0xffff) & 0xffff) >> 4;
    G3_RotX(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    transZ = -(SIDE_SWAY(side).z >> 1);
    if (side == 0) {
        transX = -SIDE_SWAY(side).x;
    } else {
        transX = SIDE_SWAY(side).x;
    }
    *REG_MTX_TRANS = -(transX >> 1);
    *REG_MTX_TRANS = 0;
    *REG_MTX_TRANS = transZ;
    Gx_SendQuadXY(side == 0 ? -640 : 0, -199, 448, 640, 128, 0, 0, 320, 64);
    *REG_MTX_POP = 1;

    *REG_POLYGON_ATTR = 0x1f0081;
    *REG_TEXIMAGE_PARAM = 0x6e430000 | (((u32)(leftSide ? 0 : 0x100) << 5) >> 3);
    *REG_PLTT_BASE = ((u32)(leftSide ? 12 : 13) << 5) >> 4;
    if (Battle_IsSideSea(side) == 1) {
        floorScroll = MainLoop_GetFrameCount() & 0xff;
        if (side == 0) {
            floorScroll = floorScroll >> 1;
        } else {
            floorScroll = -(floorScroll >> 1);
        }
        Gx_SendQuadXZ(side == 0 ? -640 : -64, 1, -448, 704, 1024, floorScroll, 0, floorScroll + 352, 512);
    } else {
        Gx_SendQuadXZ(side == 0 ? -640 : -64, 1, -448, 704, 1024, 0, 0, 352, 512);
    }

    switch (BATTLE_SIDE_UNIT_TYPE(side)) {
    case UNIT_PIPERUNNER:
    case 30:
        *REG_POLYGON_ATTR = 0x1f0081;
        *REG_TEXIMAGE_PARAM = 0x6db30000 | (((u32)gMenuState.battle.pipeTexture << 5) >> 3);
        *REG_PLTT_BASE = (14 << 5) >> 4;
        if (side == 0) {
            Gx_SendQuadXZ(-88, -1, -448, 64, 1024, 0, 0, 64, 512);
        } else {
            Gx_SendQuadXZ(24, -1, -448, 64, 1024, 64, 0, 0, 512);
        }
        break;
    }
}
