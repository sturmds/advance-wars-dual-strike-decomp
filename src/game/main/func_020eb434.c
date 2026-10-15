#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen.h"
#include "game/math.h"

#define FADE_FRAMES 80
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void Task_ResumeScript(Task* task);
#define FRAME(task) ((task)->unk_64)

void func_020eb434(Task* task) {
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = Math_Ease(0, 16, 0, FRAME(task), FADE_FRAMES);
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  FRAME(task)++;
  if (FRAME(task) > FADE_FRAMES) {
    Screen_SetLayerScreenSize(0, 0);
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    Task_ResumeScript(task);
  }
}
