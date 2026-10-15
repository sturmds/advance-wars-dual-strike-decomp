#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern UnitAction data_0229d8c8;

void Link_SendPacket(void* payload);

/* Link play: sends unit action 8 (unload) for the acting unit with its fuel. The two bytes
 * after the unit index are the caller's data_022a6b34 pair; what they hold is not worked out. */
void UnitAction_SendUnload(int unitIndex, int dropA, int dropB) {
  MapUnit* unit = &MAP_STATE->units[unitIndex];

  data_0229d8c8.unitIndex = unitIndex;
  data_0229d8c8.action = 8;
  data_0229d8c8.target = dropA;
  data_0229d8c8.unk_07 = dropB;
  data_0229d8c8.fuelLeft = unit->fuel;
  Link_SendPacket(&data_0229d8c8);
}
