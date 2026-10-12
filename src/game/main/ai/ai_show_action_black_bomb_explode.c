#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActingUnit_FinishAction(void);
void BlackBombExplosion_Start(int x, int y, int x2, int y2, int mode);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Display step of unit action 23, the Black Bomb explosion: the acting
 * unit's hp goes to 0, then BlackBombExplosion_Start draws the effect at the action's
 * tile. */
void AiShowAction_BlackBombExplode(void) {
  STATE->actingUnit->hp = 0;
  ActingUnit_FinishAction();
  BlackBombExplosion_Start(gUnitAction.x, gUnitAction.y, gUnitAction.x,
                           gUnitAction.y, 3);
}
