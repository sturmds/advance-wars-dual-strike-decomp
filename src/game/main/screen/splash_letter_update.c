#include "types.h"
#include "game/splash_task.h"
#include "game/oam_cells.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* SplashTask.letterState */
#define LETTER_DROP 0 /* height halves each frame from 80 */
#define LETTER_RISE 1
#define LETTER_FALL 2
#define LETTER_REST 3

/* Letter task update: the letter falls from 80, then bounces 10 times,
 * starting each rise with a step of 80 / (bounce + 3), rests for 120 frames
 * and starts bouncing again. Letter 3 sets restedOnce the first time it rests. */
void SplashLetter_Update(SplashTask* task) {
  switch (task->letterState) {
    case LETTER_DROP:
      task->height >>= 1;
      if (task->height == 0) {
        task->riseStep = 40;
        task->letterState++;
      }
      break;
    case LETTER_RISE:
      if (task->riseStep == 0) {
        task->letterState++;
        break;
      }
      task->riseStep >>= 1;
      task->height += task->riseStep;
      break;
    case LETTER_FALL:
      task->height >>= 1;
      if (task->height == 0) {
        task->riseStep = 80 / (task->bounces + 3);
        if (task->bounces < 10) {
          task->bounces++;
          task->letterState--;
        } else {
          task->holdFrames = task->bounces * 12;
          task->letterState++;
        }
      }
      break;
    case LETTER_REST:
      if (task->restedOnce == 0 && task->letterIndex == 3) {
        task->restedOnce++;
      }
      task->holdFrames--;
      if (task->holdFrames == 0) {
        task->bounces = 0;
        task->letterState -= 2;
      }
      break;
  }
  Oam_PutSprite(task->x, (task->y - task->height) & 0xff, &data_027c0048,
                task->letterIndex * 16 + 0x3194);
}
