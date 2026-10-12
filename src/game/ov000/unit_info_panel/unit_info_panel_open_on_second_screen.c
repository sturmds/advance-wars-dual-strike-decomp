#include "types.h"

int SecondScreenTask_IsRunning(void);
void func_ov000_022b0054(void);
void UnitInfoPanel_OpenForType(int owner, int unitType);

/* Opens the unit info panel for a unit type, but only while the second
 * screen task is running. func_ov000_022b0054 first destroys the task
 * with script data_ov000_0234eb64 on the first screen. */
void UnitInfoPanel_OpenForTypeOnSecondScreen(int owner, int unitType) {
  if (!SecondScreenTask_IsRunning()) {
    return;
  }
  func_ov000_022b0054();
  UnitInfoPanel_OpenForType(owner, unitType);
}
