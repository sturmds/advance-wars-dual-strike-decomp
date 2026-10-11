#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void SpriteAnim_Draw(struct ShowSprite *sprite, int x, int y);  /* guess: sets the sprite's screen position */

extern s16 gSineTable[];            /* looks like a 256-entry sine table */
extern s16 data_02138a68[];            /* the same table from entry 64: cosine */

/* Script step: waits unk_50 frames, then moves the task's sprite out from
 * (tweenFrom, tweenTo) in the direction unk_48 (a sine table index; 0 points
 * up), the distance easing from 0 to 512 over tweenFrames frames. Drawn on
 * the first screen when unk_38 is 0, else on the second. */
void Ov004Task_MoveSpriteOutward(Ov004Task *task)
{
    struct ShowSprite *sprite = task->sprite;
    int angle = task->unk_48;
    int distance;
    int x;
    Unk_027c0280 *savedGfx;
    int y;
    Unk_02080440 *savedState;

    switch ((u32)task->step) {         /* needed to match: unsigned compares */
    case 0:
        task->frame = 0;
        task->duration = task->unk_50;
        task->step++;
    case 1:
        if (task->frame < task->duration) {
            task->frame++;
            return;
        }
        task->step++;
    case 2:
        task->frame = 0;
        task->duration = task->tweenFrames;
        task->step++;
    case 3:
        distance = Math_Ease(4, 0, 512, task->frame, task->duration);
        x = (distance * gSineTable[angle & 0xff]) >> 12;
        x += task->tweenFrom;
        y = (distance * -data_02138a68[angle & 0xff]) >> 12;
        y += task->tweenTo;
        if (x > 288) {
            x = 288;
        }
        if (y < -16) {
            y = -16;
        }
        if (task->unk_38 == 0) {
            savedGfx = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            savedState = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            SpriteAnim_Draw(sprite, x & 0x1ff, y & 0xff);
            Screen_SetCurrent(savedState);
            Screen_SetCurrentGfx(savedGfx);
        } else {
            savedGfx = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            savedState = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            SpriteAnim_Draw(sprite, x & 0x1ff, y & 0xff);
            Screen_SetCurrent(savedState);
            Screen_SetCurrentGfx(savedGfx);
        }
        if (task->frame < task->duration) {
            task->frame++;
        } else {
            task->step = 100;
        }
        break;
    case 100: /* done; needed to match: the empty case shapes the switch */
        break;
    }
}
