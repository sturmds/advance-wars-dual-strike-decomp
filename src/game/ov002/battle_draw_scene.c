#include "types.h"
#include "sdk/fx.h"
#include "game/task.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

extern MenuState gMenuState;

#define REG_MTX_PUSH ((volatile u32*)0x04000444)
#define REG_MTX_POP ((volatile u32*)0x04000448)
#define REG_MTX_SCALE ((volatile u32*)0x0400046c)
#define REG_MTX_TRANS ((volatile u32*)0x04000470)
#define REG_VIEWPORT ((volatile u32*)0x04000580)

extern u8 data_ov002_023604ac[];
extern u8 data_ov002_023604b8[];
/* The look-at target; never written, so the origin. */
static VecFx32 data_ov002_0236bbe0;
extern s16 data_0214d4c4[];

void G3X_Reset(void);
void G3i_LookAt_(void* cam, void* up, void* target, int load, void* mtx);
void G3_RotX(int sin, int cos);
void Battle_StepSideShake(int side);
void Battle_DrawSideGround(int side);
void func_ov002_02354124(void);
void Battle_SetupCamera(void);
int func_ov002_023540a0(void);
void Battle_SetupProjection(void);
void Battle_SetSideProjection(int side);
BOOL Battle_IsSideAir(int side);
void Task_RunList(Task* task);

/* Draws the battle screen's 3D scene every frame: sets up the camera and light, runs the
 * background tasks, then draws each side in its own half of the screen (viewport) with its
 * BattleSway offset and tilt, running that side's tasks. Air units sit higher. Last, it scrolls
 * BG2 and BG3. */
void Battle_DrawScene(void) {
  int side = 0;

  G3X_Reset();
  G3i_LookAt_(data_ov002_023604ac, data_ov002_023604b8, &data_ov002_0236bbe0, 1,
              NULL);
  func_ov002_02354124();
  *REG_VIEWPORT = 0xbfff0000;
  Battle_SetupProjection();
  *REG_MTX_PUSH = side;
  Battle_SetupCamera();
  func_ov002_023540a0();
  Task_RunList(data_027c027c->taskLists[1]);
  Task_RunList(data_027c027c->taskLists[2]);
  Task_RunList(data_027c027c->taskLists[3]);

  *REG_VIEWPORT = 0xbf7f0000;
  Battle_SetSideProjection(side);
  {
    s16 z;
    int y;
    int idx;

    *REG_MTX_PUSH = side;
    z = gMenuState.battle.sway[0].z;
    if (Battle_IsSideAir(side) == 1) {
      y = gMenuState.battle.sway[0].y + 88;
    } else {
      y = gMenuState.battle.sway[0].y - 8;
    }
    *REG_MTX_TRANS = -64 - gMenuState.battle.sway[0].x;
    *REG_MTX_TRANS = y;
    *REG_MTX_TRANS = z;
    Battle_StepSideShake(0);
    idx = ((gMenuState.battle.sway[0].angle + 0x600) & 0xffff) >> 4;
    G3_RotX(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    *REG_MTX_SCALE = 0x2000;
    *REG_MTX_SCALE = 0x1000;
    *REG_MTX_SCALE = 0x1000;
    Battle_DrawSideGround(0);
    Task_RunList(data_027c027c->taskLists[4]);
  }

  *REG_MTX_POP = 1;
  *REG_VIEWPORT = 0xbfff0080;
  Battle_SetSideProjection(1);
  {
    s16 z;
    int y;
    int idx;

    *REG_MTX_PUSH = 0;
    z = gMenuState.battle.sway[1].z;
    if (Battle_IsSideAir(1) == 1) {
      y = gMenuState.battle.sway[1].y + 88;
    } else {
      y = gMenuState.battle.sway[1].y - 8;
    }
    *REG_MTX_TRANS = gMenuState.battle.sway[1].x + 64;
    *REG_MTX_TRANS = y;
    *REG_MTX_TRANS = z;
    Battle_StepSideShake(1);
    idx = ((gMenuState.battle.sway[1].angle + 0x600) & 0xffff) >> 4;
    G3_RotX(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    *REG_MTX_SCALE = 0x2000;
    *REG_MTX_SCALE = 0x1000;
    *REG_MTX_SCALE = 0x1000;
    Battle_DrawSideGround(1);
    Task_RunList(data_027c027c->taskLists[5]);
  }

  *REG_MTX_POP = 1;
  *REG_MTX_POP = 1;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] =
      gMenuState.battle.unk_40c[0] + gMenuState.battle.bgScroll[0];
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] =
      -(gMenuState.battle.unk_40c[1] + gMenuState.battle.bgScroll[1]);
}
