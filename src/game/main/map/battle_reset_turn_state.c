#include "types.h"
#include "game/map_unit.h"
#include "game/Unk_02080440.h"
#include "game/main_loop.h"

extern u16 data_0229d8c4;
u8 data_022a6b30;
u32 gMapGameRunning;
/* Per cargo slot: the unload the player picked (UnloadTask_PickTile), 0 when
 * none is pending. */
u8 data_022a6b34[MAP_UNIT_CARGO_COUNT];

void Panels_PlaceForCursor(void);
void func_020b9c3c(void);
void func_020b98bc(void);
void Units_ClearAll(void);
void MapObjects_Clear(void);
BOOL Map_IsCoSelectPhase(void);

/* Clears the map state's view and menu fields before a battle starts and sets
 * day 1, player 0. On a map where a player picks a CO first, the day starts at
 * 0. The name is a guess from the callers (battle setup hooks). */
void Battle_ResetTurnState(void) {
  MAP_STATE->cursorX = 0;
  MAP_STATE->cursorY = 0;
  MAP_STATE->cursorPixelX = 0;
  MAP_STATE->cursorPixelY = 0;
  Panels_PlaceForCursor();
  MAP_STATE->unk_10 = 0;
  MAP_STATE->unk_14 = 0;
  func_020b9c3c();
  func_020b98bc();
  data_0229d8c4 = 0;
  gMapGameRunning = 1;
  MAP_STATE->unk_18 = 4;
  MAP_STATE->unk_1c = 0;
  MAP_STATE->currentPlayer = 0;
  MAP_STATE->day = 1;
  Units_ClearAll();
  MapObjects_Clear();
  if (Map_IsCoSelectPhase()) {
    MAP_STATE->day = 0;
  }
}
