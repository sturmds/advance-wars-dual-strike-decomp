#include "types.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"

int HwDiv32(int num, int den);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void SpriteStrip_Queue(int affineIndex);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Task* task);
void TitleCall_DrawNumber(int affineIndex);

extern Unk_027c0280* data_027c0280;
extern const s16 gSineTable[];
extern const TaskScriptCmd data_0215bb88[];

/* Frame counters of this script's steps: this step's, then the next step's. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)
#define HOLD(task) (((s16*)&(task)->unk_4c)[1])

#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

/* Title call task script step (data_0215bbc8), 64 frames on the current
 * screen. Layer 0 is alpha-blended in (EVA up to 16 by frame 8, then down
 * to 1) while it scrolls from the right to the left, with one sprite
 * moving along with it. From frame 4 the sprite strip and the campaign
 * record number (TitleCall_DrawNumber) are drawn; frames 4-19 grow them from a
 * flat line to full height through affine matrix 31. Frame 8 starts the
 * data_0215bb88 task under this one. The script resumes after frame 64. */
void TitleCall_NumberSlideIn(Task* task) {
  int slide;

  FRAME(task)++;
  if (FRAME(task) < 8) {
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = Math_Ease(1, 1, 16, FRAME(task), 8);
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
  } else {
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = Math_Ease(4, 16, 1, FRAME(task) - 8, 56);
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
  }
  slide = Math_Ease(4, 184, -184, FRAME(task), 64);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = slide - 8;
  Sprite_QueueAt(0, (slide + 112) & 0x1ff, 80, &data_027c0048, 0, 64);
  if (FRAME(task) >= 4) {
    if (FRAME(task) < 20) {
      int pdDivisor;
      int pbDivisor;
      int sinValue;
      int cosValue;

      pdDivisor = Math_Ease(0, 2, 256, FRAME(task) - 4, 16) != 0
                      ? Math_Ease(0, 2, 256, FRAME(task) - 4, 16)
                      : 2;
      pbDivisor = Math_Ease(0, 2, 256, FRAME(task) - 4, 16) != 0
                      ? Math_Ease(0, 2, 256, FRAME(task) - 4, 16)
                      : 2;
      cosValue = COS0 << 4;
      sinValue = SIN0;
      Oam_SetAffineParams(
          31, HwDiv32(cosValue, 256), HwDiv32(-sinValue << 4, pbDivisor),
          HwDiv32(sinValue << 4, 256), HwDiv32(cosValue, pdDivisor));
      SpriteStrip_Queue(31);
      TitleCall_DrawNumber(31);
    } else {
      SpriteStrip_Queue(0);
      TitleCall_DrawNumber(0);
    }
  }
  if (FRAME(task) == 8) {
    Task_Create(data_0215bb88, task);
  }
  if (FRAME(task) != 64) {
    return;
  }
  HOLD(task) = 0;
  Task_ResumeScript(task);
}
