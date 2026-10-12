#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
BOOL UnitMenu_JoinHidden(void);
BOOL UnitMenu_BoardHidden(void);
void RangeMap_Fill(int value);
int func_020dc1a8(void);
#define STATE (data_027c027c->unk_48[data_027c0284])

s8 func_020bf8b8(void) {
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  RangeMap_Fill(-1);
  STATE->rangeMap[STATE->actingUnitDestY][STATE->actingUnitDestX] = 0;
  return func_020dc1a8() == 0;
}
