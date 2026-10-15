#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02017514(int slot, s16* x, s16* y);
void ScriptSlot_StepMove(int slot);

/* Script command: moves the slot with a starting speed and a constant
 * acceleration, both read from the command's argument block, for the
 * block's frame count. The first run reads the start position; every
 * run steps one frame. */
BOOL ScriptSlot_CmdMoveAccel(int slot) {
  AccelMoveArgs* args = (AccelMoveArgs*)MOVE_SLOT(slot).pc->arg;

  if (MOVE_SLOT(slot).started == 0) {
    s16 startX;
    s16 startY;

    func_02017514(slot, &startX, &startY);
    MOVE_SLOT(slot).x = startX;
    MOVE_SLOT(slot).y = startY;
    MOVE_SLOT(slot).velX = args->velX;
    MOVE_SLOT(slot).velY = args->velY;
    MOVE_SLOT(slot).accelX = args->accelX;
    MOVE_SLOT(slot).accelY = args->accelY;
    MOVE_SLOT(slot).framesLeft = args->frames;
    MOVE_SLOT(slot).started++;
  }
  ScriptSlot_StepMove(slot);
  return FALSE;
}
