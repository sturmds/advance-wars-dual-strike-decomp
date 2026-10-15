#include "types.h"
#include "game/Unk_02080440.h"

void func_020c4024(void);
void func_020c4028(void);
void MapView_ScrollStep(int step);
void Map_ScrollViewToTile(int arg);
BOOL func_020ef2f0(void);
BOOL MapTouch_IsIdle(void);
void TouchMarker_Resume(void);
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020f77bc(void);

/* Game_State15_RunStep sub-state 6: scrolls the view to the cursor tile, and
 * when both waits are done refreshes the unit tiles and returns to the free cursor. */
void Game_State15_ScrollToTileThenReturnToCursor(void) {
  func_020c4024();
  func_020c4028();
  MapView_ScrollStep(4);
  Map_ScrollViewToTile(1);
  if (!func_020ef2f0()) {
    return;
  }
  if (!MapTouch_IsIdle()) {
    return;
  }
  TouchMarker_Resume();
  UnitShowTask_DestroyRunning();
  func_020c8888();
  func_020f77bc();
  MAP_STATE->unk_1c = 0;
}
