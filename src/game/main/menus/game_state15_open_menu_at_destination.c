#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_SetTile(int x, int y, s16* tile, u16* pixel);
void UnitMenu_Open(void);

/* Game_State15_RunStep sub-state 3 (set by UnitMoveSelect_TryMoveTo): puts
 * the cursor on the destination tile, calls UnitMenu_Open (guess: opens the
 * unit menu) and moves on to sub-state 4. */
void Game_State15_OpenMenuAtDestination(void) {
  MapCursor_SetTile(MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY,
                    &MAP_STATE->cursorX, &MAP_STATE->cursorPixelX);
  UnitMenu_Open();
  MAP_STATE->unk_1c = 4;
}
