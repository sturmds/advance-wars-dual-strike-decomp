#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/play_records.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020c4024(void);
void MapCursor_UpdateFree(void);
void MapView_ScrollStep(int directions);
void Map_ScrollViewToTile(int mode);
BOOL MapView_IsAtRest(void);
u32 func_020ef2c4(void);
u32 MapTouch_ReadTileTap(void);
void Game_State15_ReturnToCursor(void);
/* Reads the cursor tile itself; x and y are passed but unused (needed to
 * match: they keep the cursor in r0 and r1 for the call). */
BOOL MapCursor_IsOnIdleOwnUnit(int x, int y);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020b8230(void);

/* Bits func_020ef2c4 (buttons) and MapTouch_ReadTileTap (touch) return. */
#define INPUT_CONFIRM 0x10
#define INPUT_CANCEL 0x20

/* Unit action 20: delete a unit. func_020b8230 plays it: map script list
 * 10 entry 47, else the unit's destroy task (func_020edb84). */
#define UNIT_ACTION_DELETE 20

/* needed to match: the original loads the index before the base. */
#define STATE_AT(index)                                             \
  (*(Unk_02080440_48**)((u8*)((index) * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &            \
                        ((Unk_02080440*)0)->unk_48))

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game_State15_RunStep sub-state 2: the player picks one of their own units
 * to delete. Confirm on a unit of the current player that has not waited
 * deletes it (sending action 20 in link play) and counts it in
 * gPlayRecords for a human player; confirm on any other tile, or cancel,
 * goes back to the free cursor. Confirm off the map does nothing. */
void Game_State15_PickUnitToDelete(void) {
  u16 buttons;
  u16 input;
  int slot;
  MapUnit* unit;
  Unk_02080440_48* state;
  int x;
  int y;

  func_020c4024();
  MapCursor_UpdateFree();
  MapView_ScrollStep(4);
  Map_ScrollViewToTile(5);
  if (!MapView_IsAtRest()) {
    return;
  }
  buttons = func_020ef2c4();
  input = buttons | MapTouch_ReadTileTap();
  if (input & INPUT_CANCEL) {
    Game_State15_ReturnToCursor();
    return;
  }
  if (!(input & INPUT_CONFIRM)) {
    return;
  }
  state = STATE_AT(data_027c0284);
  x = state->cursorX;
  if (x < 0 || x >= state->unk_1c4->mapWidth) {
    return;
  }
  y = state->cursorY;
  if (y < 0 || y >= state->unk_1c4->mapHeight) {
    return;
  }
  if (MapCursor_IsOnIdleOwnUnit(x, y)) {
    slot =
        GAME->mapTiles
            ->unitAt[GAME->mapTiles->rowStart[GAME->cursorY] + GAME->cursorX];
    unit = &GAME->units[slot];
    if (gLinkPlay != 0) {
      UnitAction_Send(UNIT_ACTION_DELETE, (u8)slot, 0, 0);
    }
    GAME->actingUnit = unit;
    func_020b8230();
    if (GAME->players[GAME->currentPlayer].controller ==
        PLAYER_CONTROLLER_HUMAN) {
      if (gPlayRecords.unitsDeleted < 999999) {
        gPlayRecords.unitsDeleted++;
      }
    }
  } else {
    Game_State15_ReturnToCursor();
  }
}
