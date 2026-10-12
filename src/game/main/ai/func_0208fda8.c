#include "types.h"
#include "game/unit_action.h"

extern UnitAction data_0229d8c8;

void Link_SendPacket(UnitAction* action);

/* Copies gUnitAction to data_0229d8c8, except pending, and passes it to
 * Link_SendPacket. Purpose of the copy unknown. */
void func_0208fda8(void) {
  int i;

  data_0229d8c8.action = gUnitAction.action;
  data_0229d8c8.x = gUnitAction.x;
  data_0229d8c8.y = gUnitAction.y;
  data_0229d8c8.fromX = gUnitAction.fromX;
  data_0229d8c8.fromY = gUnitAction.fromY;
  data_0229d8c8.unitIndex = gUnitAction.unitIndex;
  data_0229d8c8.rngState = gUnitAction.rngState;
  data_0229d8c8.target = gUnitAction.target;
  data_0229d8c8.unk_07 = gUnitAction.unk_07;
  data_0229d8c8.fuelLeft = gUnitAction.fuelLeft;
  for (i = 0; i < 6; i++) {
    data_0229d8c8.packed[i] = gUnitAction.packed[i];
  }
  Link_SendPacket(&data_0229d8c8);
}
