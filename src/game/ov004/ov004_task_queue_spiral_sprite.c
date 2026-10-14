#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/oam_cells.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern s16 gSineTable[];

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Ov004Task* task);

/* Script step: a sprite that spirals out. Step 1 waits tweenFrames frames,
 * step 2 then runs for unk_48 frames. Each frame the radius eases from 0
 * to 512 and the angle (tweenFrames, a sine table index) advances by unk_4c.
 * The position is the point (tweenFrom, tweenTo) plus the eased radius along
 * a fixed direction, plus unk_50 along the current angle, and is queued as a
 * sprite. Does nothing while the slot state is 2. */
void Ov004Task_QueueSpiralSprite(Ov004Task* task) {
  int radius;
  int x;
  int y;

  if (data_ov004_022bf158->slotState[task->slot] == 2) {
    return;
  }
  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1:
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->frame = 0;
        task->duration = task->unk_48;
        task->tweenFrames = 0;
        task->step++;
      }
      break;
    case 2:
      radius = Math_Ease(4, 0, 512, task->frame, task->duration);
      task->tweenFrames += task->unk_4c;
      x = 0;
      y = (task->unk_50 * -gSineTable[task->tweenFrames & 0xff]) >> 12;
      x += (radius * gSineTable[48]) >> 12;
      y += (radius * -gSineTable[112]) >> 12;
      x += task->tweenFrom;
      y += task->tweenTo;
      if (x > 288) {
        x = 288;
      }
      if (y < -16) {
        y = -16;
      }
      if (task->unk_38 == 0) {
        Sprite_Queue(0, x & 0x1ff, y & 0xff, &data_027c0040, 0x400, 0);
      } else {
        Sprite_Queue(0, x & 0x1ff, y & 0xff, &data_027c0028, 0x400, 4);
      }
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
