#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActingUnit_SaveStartPos(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void func_020b9d48(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Removes the exploding Black Bomb: sets its hp to 0, then runs the usual
 * end-of-action steps for the acting unit. task is not used. */
void BlackBomb_RemoveActingUnit(Task* task) {
  GAME->actingUnit->hp = 0;
  ActingUnit_SaveStartPos();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  func_020b9d48();
}
