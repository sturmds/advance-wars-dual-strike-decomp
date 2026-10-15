#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"

void PlaySuperPowerTheme(int player);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* BG0 scroll where the banner rests after sliding in. */
#define LAYER0_REST_Y (-52)

/* Step of the Super CO Power cut-in after the banner on BG0 has slid in:
 * bounces it up 8, 4 and 2 pixels over 15 frames, then plays the Super CO
 * Power theme. The bg index is read before each Math_Ease call to match. */
void SuperBreak_BounceBanner(Task* task) {
  int frame = FRAME(task);
  u8 bg;

  if (frame < 4) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] = Math_Ease(0, 0, 8, frame, 4) + LAYER0_REST_Y;
  } else if (frame < 8) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] =
        Math_Ease(0, 8, 0, frame - 4, 4) + LAYER0_REST_Y;
  } else if (frame < 10) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] =
        Math_Ease(0, 0, 4, frame - 8, 2) + LAYER0_REST_Y;
  } else if (frame < 12) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] =
        Math_Ease(0, 4, 0, frame - 10, 2) + LAYER0_REST_Y;
  } else if (frame < 13) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] =
        Math_Ease(0, 0, 2, frame - 12, 1) + LAYER0_REST_Y;
  } else if (frame <= 14) {
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] =
        Math_Ease(0, 2, 0, frame - 13, 1) + LAYER0_REST_Y;
  } else {
    PlaySuperPowerTheme(MAP_STATE->currentPlayer);
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
  FRAME(task)++;
}
