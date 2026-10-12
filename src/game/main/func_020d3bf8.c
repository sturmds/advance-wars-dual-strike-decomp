#include "types.h"

void Battle_RunSetupHooks(void);
void Map_LoadPredeployedUnits(void);
void Battle_RegisterFrameCallbacks(void);
void func_020e88f8(void);

void func_020d3bf8(void) {
  Battle_RunSetupHooks();
  Map_LoadPredeployedUnits();
  Battle_RegisterFrameCallbacks();
  func_020e88f8();
}
