#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern TaskScriptCmd data_ov004_022bdfd8[];
extern TaskScriptCmd data_ov004_022be038[];

void Task_ResumeScript(void* task);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad(int x, int y, int z, int sSize, int tSize, int w,
                            int h, int s0, int t0, u32 texAddr, u32 palAddr);
int func_ov004_022ada10(void);
void func_ov004_022ba98c(Unk_ov004_022bf158* state);
void func_ov004_022bb1d8(Unk_ov004_022bf158* state);
void Ov004_SetSlotTask(Unk_ov004_022bf158* state, void* task, int id);
void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state);
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int cue, int time);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void* PaletteFade_StartFromBlack(int a, void* b, int c, int d, int e);

/* Runs a timeline of cues (frames 0, 30 and 222 by Ov004_CheckTimedEvent, each fires once):
 * 0 sets up the layers and starts two tasks, 30 starts another, 222 blacks the screen
 * out and resumes the script. Until then it draws two 256x128 textured quads every
 * frame. task->duration counts the task slots handed out. */
void Ov004Task_RunBannerTimeline(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  Ov004_UpdateElapsedFrames(state);
  if (Ov004_CheckTimedEvent(state, 0, 0)) {
    Unk_027c0280* savedGfx;
    Unk_02080440* savedState;
    int id;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 0;
    GFX_DISPCNT(data_027c0280)->bg2 = 1;
    GFX_DISPCNT(data_027c0280)->bg3 = 0;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 80;
    data_027c0280->masterBrightness = 0;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    state->unk_8f8 = 0;
    id = task->duration++;
    Ov004_SetSlotTask(state, Task_Create(data_ov004_022bdfd8, TASK_LIST(5)),
                      id);
    id = task->duration++;
    Ov004_SetSlotTask(state, Task_Create(data_ov004_022be038, TASK_LIST(5)),
                      id);
  }
  if (Ov004_CheckTimedEvent(state, 1, 30)) {
    int id;

    id = task->duration++;
    Ov004_SetSlotTask(
        state, PaletteFade_StartFromBlack(5, state->savedPalette, 70, 0, 8),
        id);
  }
  if (Ov004_CheckTimedEvent(state, 2, 222)) {
    Unk_027c0280* savedGfx;
    Unk_02080440* savedState;

    Ov004_StartClock(state);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->masterBrightness = -16;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    func_ov004_022ba98c(state);
    func_ov004_022bb1d8(state);
    Task_ResumeScript(task);
  } else {
    Ov004_Init3d();
    Ov004_SendTexturedQuad(state->unk_8f8 + 16, 16, 0, 5, 4, 256, 128, 0, 0,
                           0x8000, 160);
    Ov004_SendTexturedQuad(state->unk_8f8 + 272, 16, 0, 5, 4, 256, 128, 0, 0,
                           0xc000, 160);
    func_ov004_022ada10();
  }
}
