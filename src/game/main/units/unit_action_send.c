#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern UnitAction data_0229d8c8;
extern u8 data_0229d8d4[];

void Link_SendPacket(void* p);
void PackNibblePairs(const u8* nibbles, u8* out);

/* Copies the current action into data_0229d8c8 and hands it to Link_SendPacket, which sends a packet
 * with a counter (probably link play). Used when gLinkPlay is set. */
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg) {
  MapUnit* unit = &MAP_STATE->units[unitIndex];

  data_0229d8c8.action = action;
  data_0229d8c8.x = MAP_STATE->actingUnitDestX;
  data_0229d8c8.y = MAP_STATE->actingUnitDestY;
  data_0229d8c8.fromX = MAP_STATE->actingUnitStartX;
  data_0229d8c8.fromY = MAP_STATE->actingUnitStartY;
  data_0229d8c8.unitIndex = unitIndex;
  data_0229d8c8.target = target;
  data_0229d8c8.unk_07 = arg;
  data_0229d8c8.fuelLeft = unit->fuel;
  PackNibblePairs(MAP_STATE->pathSteps, data_0229d8d4);
  Link_SendPacket(&data_0229d8c8);
}
