#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
void func_02017514(int slot, s16* x, s16* y);
void ScriptSlot_StepMove(int slot);

/* Script command: moves the slot horizontally at a constant speed. The
 * command's word argument is the speed per frame (an integer), its
 * 16-bit argument the frame count. The first run reads the start
 * position; every run steps one frame. */
BOOL ScriptSlot_CmdMoveX(int slot) {
  if (MOVE_SLOT(slot).started == 0) {
    s16 startX;
    s16 startY;

    func_02017514(slot, &startX, &startY);
    MOVE_SLOT(slot).x = startX;
    MOVE_SLOT(slot).y = startY;
    MOVE_SLOT(slot).velX = (int)MOVE_SLOT(slot).pc->arg;
    MOVE_SLOT(slot).velY = 0;
    MOVE_SLOT(slot).accelX = 0;
    MOVE_SLOT(slot).accelY = 0;
    MOVE_SLOT(slot).framesLeft = MOVE_SLOT(slot).pc->arg16;
    MOVE_SLOT(slot).started++;
  }
  ScriptSlot_StepMove(slot);
  return FALSE;
}
