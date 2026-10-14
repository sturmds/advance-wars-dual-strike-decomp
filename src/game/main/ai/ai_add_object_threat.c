#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_object.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Ai_UnitUsesThreatMaps(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* With objects attackable (AiTargetFlags) and a unit type that cares
 * (Ai_UnitUsesThreatMaps), points the range map at threatSum and hands each active
 * object (state 1) to MapObject_MarkFireRange with -1. */
void Ai_AddObjectThreat(void) {
  MapObject* object;

  if (!AI_TARGET_FLAGS->attackObjects) {
    return;
  }
  if (!Ai_UnitUsesThreatMaps(STATE->actingUnit->type)) {
    return;
  }
  RangeMap_SetBuffer(STATE->mapTiles->threatSum);
  for (object = STATE->objects; object->kind != MAP_OBJECT_NONE; object++) {
    if (object->state == 1) {
      MapObject_MarkFireRange(object, -1);
    }
  }
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
}
