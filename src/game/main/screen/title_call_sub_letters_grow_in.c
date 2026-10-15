#include "types.h"
#include "game/oam_cells.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"
#include "game/oam_buffer.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
Task* Task_FindByScript(const void* script);
void Task_ResumeScript(Task* task);

extern const u8 data_0215bb60[];
extern const TaskScriptCmd data_0215bc10[];

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Sprites on the sub screen; letter i uses OBJ affine matrix i + 1 while it grows. */
#define LETTER_COUNT 8

/* Title call task script step on the sub screen. Eight sprites at x
 * positions data_0215bb60 (tiles 32 apart) grow in from a vertical line to
 * full width, each over 20 frames and 4 frames after the one before. At
 * frame 84 it starts the number task (TitleCall_StartNumberTask) under the
 * title call task (data_0215bc10) and resumes the script. */
void TitleCall_SubLettersGrowIn(Task* task) {
  Unk_027c0280* saved0280;
  int i;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (i = 0; i < LETTER_COUNT; i++) {
    if (FRAME(task) >= i * 4) {
      if (FRAME(task) < i * 4 + 20) {
        int scale;
        int d1;
        int d0;

        scale = Math_Ease(0, 2, 256, FRAME(task) - i * 4, 20);
        d1 = scale != 0 ? scale : 2;
        d0 = scale != 0 ? scale : 2;
        Oam_SetAffineParams(i + 1, HwDiv32(gSineTable[64] << 4, d0),
                            HwDiv32(-gSineTable[0] << 4, 256),
                            HwDiv32(gSineTable[0] << 4, d1),
                            HwDiv32(gSineTable[64] << 4, 256));
        Sprite_Queue(0, (data_0215bb60[i] & 0x1ff) | ((i + 1) << 9), 320,
                     &data_027c0018, 0, i * 32);
      } else {
        Sprite_Queue(0, data_0215bb60[i] & 0x1ff, 320, &data_027c0018, 0,
                     i * 32);
      }
    }
  }
  Screen_SetCurrentGfx(saved0280);
  if (FRAME(task) != 84) {
    FRAME(task)++;
    return;
  }
  TitleCall_StartNumberTask(Task_FindByScript(data_0215bc10));
  FRAME(task) = 0;
  Task_ResumeScript(task);
}
