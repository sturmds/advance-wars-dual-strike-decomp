#include "types.h"
#include "game/splash_task.h"

void Oam_PutSprite(int x, int y, void *shape, int tileAndPalette);

extern u8 data_027c0048[];

/* SplashTask.unk_30 of a letter task. unk_33 is the letter's height above
 * y, unk_35 the bounces made. */
#define LETTER_DROP 0                  /* height halves each frame from 80 */
#define LETTER_RISE 1
#define LETTER_FALL 2
#define LETTER_REST 3

/* Letter task update: the letter falls from 80, then bounces 10 times,
 * starting each rise with a step of 80 / (bounce + 3), rests for 120 frames
 * and starts bouncing again. Letter 3 sets unk_37 the first time it rests. */
void SplashLetter_Update(SplashTask *task)
{
    switch (task->unk_30) {
    case LETTER_DROP:
        task->unk_33 >>= 1;
        if (task->unk_33 == 0) {
            task->riseStep = 40;
            task->unk_30++;
        }
        break;
    case LETTER_RISE:
        if (task->riseStep == 0) {
            task->unk_30++;
            break;
        }
        task->riseStep >>= 1;
        task->unk_33 += task->riseStep;
        break;
    case LETTER_FALL:
        task->unk_33 >>= 1;
        if (task->unk_33 == 0) {
            task->riseStep = 80 / (task->unk_35 + 3);
            if (task->unk_35 < 10) {
                task->unk_35++;
                task->unk_30--;
            } else {
                task->holdFrames = task->unk_35 * 12;
                task->unk_30++;
            }
        }
        break;
    case LETTER_REST:
        if (task->unk_37 == 0 && task->letterIndex == 3) {
            task->unk_37++;
        }
        task->holdFrames--;
        if (task->holdFrames == 0) {
            task->unk_35 = 0;
            task->unk_30 -= 2;
        }
        break;
    }
    Oam_PutSprite(task->x, (task->y - task->unk_33) & 0xff, data_027c0048,
                  task->letterIndex * 16 + 0x3194);
}
