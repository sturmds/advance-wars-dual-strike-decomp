#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02017514(int slot, s16* x, s16* y);
void func_02017518(int slot, int x, int y);

/* Needed to match: a plain cast of scriptSlots makes the compiler share
 * the slot address between the two sides of `+=`; the original does not. */
#define ARC_SLOT(slot) ((*(ArcMoveSlot**)&data_027c027c->scriptSlots)[slot])

/* Script command: moves the slot's position along a parabola to an offset
 * from where it starts. The first run reads the start position and works
 * out the frame count from the horizontal speed, and the vertical speed
 * that reaches the target under the given gravity. Each run then steps
 * one frame; after the last one it snaps to the target and moves on to
 * the next command. */
BOOL ScriptSlot_CmdMoveArc(int slot) {
  ArcMoveArgs* args = (ArcMoveArgs*)ARC_SLOT(slot).pc->arg;
  s16 frames;

  if (ARC_SLOT(slot).started == 0) {
    s16 startX;
    s16 startY;

    func_02017514(slot, &startX, &startY);
    ARC_SLOT(slot).x = startX;
    ARC_SLOT(slot).y = startY;
    ARC_SLOT(slot).targetX = args->dx + ARC_SLOT(slot).x;
    ARC_SLOT(slot).targetY = args->dy + ARC_SLOT(slot).y;
    ARC_SLOT(slot).velX = args->velX;
    ARC_SLOT(slot).accelX = 0;
    ARC_SLOT(slot).accelY = args->gravity;
    frames = (ARC_SLOT(slot).targetX - ARC_SLOT(slot).x) / ARC_SLOT(slot).velX;
    if (frames < 0) {
      frames = -frames;
    }
    ARC_SLOT(slot).velY = (frames - 1) * (0.0f - ARC_SLOT(slot).accelY) / 2.0f +
                          (ARC_SLOT(slot).targetY - ARC_SLOT(slot).y) / frames;
    ARC_SLOT(slot).framesLeft = frames;
    ARC_SLOT(slot).started++;
  }
  ARC_SLOT(slot).x = ARC_SLOT(slot).x + ARC_SLOT(slot).velX;
  ARC_SLOT(slot).y = ARC_SLOT(slot).y + ARC_SLOT(slot).velY;
  ARC_SLOT(slot).velX = ARC_SLOT(slot).velX + ARC_SLOT(slot).accelX;
  ARC_SLOT(slot).velY = ARC_SLOT(slot).velY + ARC_SLOT(slot).accelY;
  func_02017518(slot, ARC_SLOT(slot).x, ARC_SLOT(slot).y);
  if (ARC_SLOT(slot).framesLeft-- == 0) {
    ARC_SLOT(slot).started = 0;
    ARC_SLOT(slot).pc++;
    func_02017518(slot, ARC_SLOT(slot).targetX, ARC_SLOT(slot).targetY);
  }
  return FALSE;
}
