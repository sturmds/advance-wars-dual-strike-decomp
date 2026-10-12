#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define FADE_FRAMES 80

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void Task_ResumeScript(Task* task);

/* Frame counter, kept in Task.unk_64. */
#define FRAME(task) ((task)->unk_64)

/* Task update: alpha-blends the screen for FADE_FRAMES frames, the first
 * target's weight easing from 16 down to 0 against a second weight of 16.
 * Afterwards it resets layer 0's screen size, requests a copy of layer 0
 * and resumes the script. */
void BlendFadeOut_Update(Task* task) {
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
