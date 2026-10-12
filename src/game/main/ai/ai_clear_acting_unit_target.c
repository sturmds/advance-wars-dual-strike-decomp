#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Clears the acting unit's AI target when it is a foot unit (infantry or
 * mech). Called before the AI queues an attack. */
void Ai_ClearActingUnitTarget(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->type > UNIT_MECH) {
    return;
  }
  data_027c027c->unk_48[data_027c0284]->actingUnit->aiTargetX =
      MAP_UNIT_NO_TARGET;
  data_027c027c->unk_48[data_027c0284]->actingUnit->aiTargetY =
      MAP_UNIT_NO_TARGET;
}
