#include "types.h"
#include "game/Unk_02080440.h"

u8 func_020b9cf4(void);
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
BOOL MapCursor_SelectTile(int x, int y, int allowBuild, int allowMenu);

/* Game_State15_RunStep sub-state 4. While func_020b9cf4 returns 0 (the flag
 * func_020b9d48 sets when a unit command runs) the unit is picked again
 * from its tile; once the flag is set the state goes back to the free
 * cursor. */
void Game_State15_AwaitUnitCommand(void) {
  if (func_020b9cf4() == 0) {
    UnitShowTask_DestroyRunning();
    func_020c8888();
    MapCursor_SelectTile(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                         TRUE, TRUE);
  } else {
    MAP_STATE->unk_1c = 0;
  }
}
