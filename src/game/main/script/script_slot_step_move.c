#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
void func_02017518(int slot, int x, int y);

/* One frame of a move command: adds the speed to the position and the
 * acceleration to the speed, then passes the new position on. After the
 * last frame the slot moves on to the next command. */
void ScriptSlot_StepMove(int slot) {
  MOVE_SLOT(slot).x = MOVE_SLOT(slot).x + MOVE_SLOT(slot).velX;
  MOVE_SLOT(slot).y = MOVE_SLOT(slot).y + MOVE_SLOT(slot).velY;
  MOVE_SLOT(slot).velX = MOVE_SLOT(slot).velX + MOVE_SLOT(slot).accelX;
  MOVE_SLOT(slot).velY = MOVE_SLOT(slot).velY + MOVE_SLOT(slot).accelY;
  func_02017518(slot, MOVE_SLOT(slot).x, MOVE_SLOT(slot).y);
  if (MOVE_SLOT(slot).framesLeft-- == 0) {
    MOVE_SLOT(slot).started = 0;
    MOVE_SLOT(slot).pc++;
  }
}
