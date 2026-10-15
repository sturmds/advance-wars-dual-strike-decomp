#include "types.h"
#include "game/title_call_task.h"
#include "game/screen.h"

void func_020517d8(void);

/* Title call script step: draws the sprite strip and the campaign record
 * number at rest, every frame. After 352 frames it starts the
 * data_0215bbe8 task (func_020517d8). */
void TitleCall_HoldNumber(TitleCallTask* task) {
  SpriteStrip_Queue(0);
  TitleCall_DrawNumber(0);
  if (task->hold == 352) {
    func_020517d8();
  }
  task->hold++;
}
