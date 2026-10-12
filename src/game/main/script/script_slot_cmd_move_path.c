#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
void func_02017514(int slot, s16* x, s16* y);
void func_02017518(int slot, int x, int y);

/* Script command: walks the slot through a list of points, one per
 * frame, each an offset from the start position. The command's word
 * argument points at the list of dx, dy pairs, ended by
 * dx == PATH_END; `started` counts the points done. */
BOOL ScriptSlot_CmdMovePath(int slot) {
  s16* point; /* dx, dy */

  if (MOVE_SLOT(slot).started == 0) {
    s16 startX;
    s16 startY;

    func_02017514(slot, &startX, &startY);
    MOVE_SLOT(slot).x = startX;
    MOVE_SLOT(slot).y = startY;
  }
  point = &((s16*)MOVE_SLOT(slot).pc->arg)[MOVE_SLOT(slot).started * 2];
  if (point[0] == PATH_END) {
    MOVE_SLOT(slot).pc++;
    MOVE_SLOT(slot).started = 0;
  } else {
    func_02017518(slot, point[0] + MOVE_SLOT(slot).x,
                  point[1] + MOVE_SLOT(slot).y);
    MOVE_SLOT(slot).started++;
  }
  return FALSE;
}
