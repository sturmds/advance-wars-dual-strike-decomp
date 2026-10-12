#include "types.h"

void func_020c4024(void);
void func_020c40d0(void);
void MapView_ScrollStep(int step);
BOOL MapView_IsAtRest(void);

/* Game_State15_RunStep sub-state 7: scrolls the view; never leaves the state. */
void Game_State15_ScrollView(void) {
  func_020c4024();
  func_020c40d0();
  MapView_ScrollStep(4);
  MapView_IsAtRest();
}
