#include "types.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/brightness_fade.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Palette_Flush(void);
void Task_ResumeScript(Ov004Task* task);

void Ov004Task_FadeBothHoldFade(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch ((u32)task->step) {
    case 0:
      state->unk_810 = func_020fcdc8(task, 30);
      state->unk_814 = func_020fcda0(task, 30);
      task->step++;
    case 1:
      if (func_020fd624(state->unk_810)) {
        return;
      }
      if (func_020fd624(state->unk_814)) {
        return;
      }
      func_020fd618(state->unk_810);
      func_020fd618(state->unk_814);
      state->unk_810 = NULL;
      state->unk_814 = NULL;
      task->frame = 0;
      task->duration = 60;
      task->step = 10;
    case 10:
      if (task->frame < task->duration) {
        task->frame++;
        return;
      }
      task->step = 20;
    case 20:
      state->unk_810 = BrightnessFade_StartOutBlackScreen0(task, 50);
      state->unk_814 = func_020fcd50(task, 50);
      task->step++;
    case 21:
      if (func_020fd624(state->unk_810)) {
        return;
      }
      if (func_020fd624(state->unk_814)) {
        return;
      }
      func_020fd618(state->unk_810);
      func_020fd618(state->unk_814);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      Mem_Zero32(0, data_027c0280->paletteBuffer, 0x400);
      Palette_Flush();
      data_027c0280->masterBrightness = 0;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      Task_ResumeScript(task);
      break;
  }
}
