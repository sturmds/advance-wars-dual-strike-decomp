#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"

#define GXFIFO_MTX_PUSH ((volatile u32*)0x04000444)
#define GXFIFO_MTX_POP ((volatile u32*)0x04000448)
#define GXFIFO_MTX_TRANS ((volatile u32*)0x04000470)

void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int event, int frame);
void Ov004_SetSlotTask(Unk_ov004_022bf158* state, Task* task, int slot);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w,
                            int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022bb1d8(Unk_ov004_022bf158* state);
/* name: a resource name, or a resource id below 0x10000 cast to a pointer. */
void Ov004_CopyResource(const char* name, void* dest);
void Ov004_UnpackResource(const char* name, void* dest);
void Ov004_BlitBlock(Unk_ov004_022bf158* state, int col, int row,
                     const char* name);
void Ov004_FillBlock(Unk_ov004_022bf158* state, int col, int row, int fill);
void Ov004_InitTileMap(Unk_ov004_022bf158* state, int cols, int rows);
Task* PaletteFade_StartFromBlack(int a, void* b, int c, int d, int e);
void G3_RotY(int sin, int cos);
void func_0200270c(int x, int y, int attr, int list, int hFlip);
void Palette_Flush(void);
void Task_ResumeScript(void* task);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern s16 data_0214d4c4[];

extern TaskScriptCmd data_ov004_022be0f8[];
extern TaskScriptCmd data_ov004_022be458[];
extern TaskScriptCmd data_ov004_022be538[];
extern TaskScriptCmd data_ov004_022be698[];
extern TaskScriptCmd data_ov004_022be778[];
extern TaskScriptCmd data_ov004_022be498[];
extern TaskScriptCmd data_ov004_022be5b8[];
extern TaskScriptCmd data_ov004_022be6f8[];
extern TaskScriptCmd data_ov004_022be7d8[];
extern TaskScriptCmd data_ov004_022be4f8[];
extern TaskScriptCmd data_ov004_022be618[];
extern TaskScriptCmd data_ov004_022bdfb8[];
extern TaskScriptCmd data_ov004_022be658[];
extern char data_ov004_022bdd48[];
extern char data_ov004_022bdd74[];
extern char data_ov004_022bdd78[];
extern char data_ov004_022bdd7c[];
extern char data_ov004_022bddc4[];
extern char data_ov004_022bddc8[];

/* Puts newTask in the next free slot. In this script the task's duration
 * field counts the slots used so far. */
#define ADD_SLOT_TASK(newTask)               \
  do {                                       \
    int slot = task->duration++;             \
    Ov004_SetSlotTask(state, newTask, slot); \
  } while (0)

/* The screen that shows the 3D engine (BG0 in mode 1, Ov004Task_InitScene). */
#define SCREEN_3D (data_027c0288.unk_00)

/* Script step that plays a timed sequence on the 3D screen. Three times it
 * loads new OBJ graphics, then moves and turns a textured quad about the Y
 * axis, with a sprite beside it; slot tasks run each move. At frame 195 it
 * fills BG1 with a 3 by 3 grid of pictures; at frame 219 it resumes the
 * script. Each frame it draws the quad and the sprite. */
void Ov004Task_PlayTurningQuadScene(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  int idx;
  int x;
  int z;

  Ov004_UpdateElapsedFrames(state);

  /* Each screen switch keeps its saved pointers in its own block: with
     * one pair for the whole function the registers do not match. */
  if (Ov004_CheckTimedEvent(state, 0, 0)) {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedState;
    Screen_SetCurrentGfx(SCREEN_3D);
    savedState = data_027c027c;
    Screen_SetCurrent(SCREEN_3D);
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 0;
    GFX_DISPCNT(data_027c0280)->bg2 = 0;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    data_027c0280->masterBrightness = 0;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    ADD_SLOT_TASK(
        PaletteFade_StartFromBlack(5, &state->savedPalette[16 * 8], 30, 8, 8));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be0f8, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 2, 23)) {
    Ov004_CopyResource((const char*)0x100c, data_027c0280->objVram);
    Ov004_CopyResource((const char*)0x200c,
                       data_027c0280->paletteBuffer + 0x200);
    Palette_Flush();
  }

  if (Ov004_CheckTimedEvent(state, 3, 25)) {
    state->unk_8f8 = 1;
    state->unk_8fc = 128;
    state->unk_900 = -80;
    state->unk_904 = 0;
    state->unk_908 = -96;
    state->unk_90c = 0;
    ADD_SLOT_TASK(Task_Create(data_ov004_022be458, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be538, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be698, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 4, 51)) {
    state->unk_8f8 = 1;
    state->unk_8fc = -64;
    state->unk_900 = 0;
    state->unk_904 = -0x4400;
    ADD_SLOT_TASK(Task_Create(data_ov004_022be778, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 10, 80)) {
    Ov004_CopyResource((const char*)0x100e, data_027c0280->objVram);
    Ov004_CopyResource((const char*)0x200e,
                       data_027c0280->paletteBuffer + 0x200);
    Palette_Flush();
    state->unk_8f8 = 2;
    state->unk_8fc = -256;
    state->unk_900 = -80;
    state->unk_904 = 0;
    state->unk_908 = 208;
    state->unk_90c = 0;
    ADD_SLOT_TASK(Task_Create(data_ov004_022be498, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be5b8, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be6f8, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 11, 110)) {
    state->unk_8f8 = 2;
    state->unk_8fc = -64;
    state->unk_900 = 0;
    state->unk_904 = 0x4400;
    ADD_SLOT_TASK(Task_Create(data_ov004_022be7d8, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 20, 140)) {
    Ov004_CopyResource((const char*)0x1019, data_027c0280->objVram);
    Ov004_CopyResource((const char*)0x2019,
                       data_027c0280->paletteBuffer + 0x200);
    Palette_Flush();
    state->unk_8f8 = 3;
    state->unk_8fc = 128;
    state->unk_900 = -80;
    state->unk_904 = 0;
    state->unk_908 = -96;
    state->unk_90c = 0;
    ADD_SLOT_TASK(Task_Create(data_ov004_022be4f8, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be618, TASK_LIST(5)));
    ADD_SLOT_TASK(Task_Create(data_ov004_022be6f8, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 21, 170)) {
    state->unk_8f8 = 3;
    state->unk_8fc = -64;
    state->unk_900 = 0;
    state->unk_904 = -0x4400;
    ADD_SLOT_TASK(Task_Create(data_ov004_022bdfb8, TASK_LIST(5)));
  }

  if (Ov004_CheckTimedEvent(state, 25, 195)) {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedState;
    Screen_SetCurrentGfx(SCREEN_3D);
    savedState = data_027c027c;
    Screen_SetCurrent(SCREEN_3D);
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 1;
    GFX_DISPCNT(data_027c0280)->bg2 = 0;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0xff00;
    Ov004_UnpackResource(
        data_ov004_022bdd74,
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14));
    Ov004_CopyResource(data_ov004_022bdd48,
                       data_027c0280->paletteBuffer + 0x20);
    Palette_Flush();
    Ov004_InitTileMap(state, 3, 3);
    Ov004_FillBlock(state, 0, 0, 0);
    Ov004_FillBlock(state, 1, 0, 0);
    Ov004_FillBlock(state, 2, 0, 0);
    Ov004_BlitBlock(state, 0, 1, data_ov004_022bddc8);
    Ov004_BlitBlock(state, 1, 1, data_ov004_022bddc4);
    Ov004_FillBlock(state, 2, 1, 0);
    Ov004_BlitBlock(state, 0, 2, data_ov004_022bdd78);
    Ov004_BlitBlock(state, 1, 2, data_ov004_022bdd7c);
    Ov004_FillBlock(state, 2, 2, 0);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    ADD_SLOT_TASK(Task_Create(data_ov004_022be658, TASK_LIST(5)));
    ADD_SLOT_TASK(BrightnessFade_StartOutBlackScreen0(task, 20));
  }

  if (Ov004_CheckTimedEvent(state, 26, 219)) {
    Ov004_StartClock(state);
    func_ov004_022bb1d8(state);
    Task_ResumeScript(task);
    return;
  }

  Ov004_Init3d();
  x = state->unk_8fc;
  z = state->unk_900;
  idx = (state->unk_904 & 0xffff) >> 4;
  *GXFIFO_MTX_PUSH = 0;
  G3_RotY(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
  *GXFIFO_MTX_TRANS = x;
  *GXFIFO_MTX_TRANS = -16;
  *GXFIFO_MTX_TRANS = z;
  /* unk_8f8 picks the 128 by 32 part of the texture the quad shows. */
  switch (state->unk_8f8) {
    case 0:
      break;
    case 1:
      Ov004_SendTexturedQuad(0, 0, 0, 5, 5, 128, 32, 128, 96, 0, 128);
      break;
    case 2:
      Ov004_SendTexturedQuad(0, 0, 0, 5, 5, 128, 32, 0, 128, 0, 128);
      break;
    case 3:
      Ov004_SendTexturedQuad(0, 0, 0, 5, 5, 128, 32, 128, 128, 0, 128);
      break;
  }
  *GXFIFO_MTX_POP = 1;
  func_ov004_022ada10();

  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedState;
    Screen_SetCurrentGfx(SCREEN_3D);
    savedState = data_027c027c;
    Screen_SetCurrent(SCREEN_3D);
    func_0200270c(state->unk_908 & 0x1ff, 160, 0x400, 0, state->unk_90c);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
  }
}
