#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiUnit_PerformAction(int x, int y, u8 action, u8 target, u8 arg);

/* AiUnit_ActByOrder runs this for units with AI order 7: the unit stays
 * on its tile (action 2) without running any AI step. */
void AiUnit_ActOrder7(void) {
  MapUnit* unit = data_027c027c->unk_48[data_027c0284]->actingUnit;

  AiUnit_PerformAction(unit->x, unit->y, 2, 0, 0);
}
