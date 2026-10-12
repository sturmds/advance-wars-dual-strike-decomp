#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Game_State15_FreeCursor(void);
void UnitMoveSelect_Update(void);
void Game_State15_PickUnitToDelete(void);
void Game_State15_OpenMenuAtDestination(void);
void Game_State15_AwaitUnitCommand(void);
void Game_State15_EndBlockedMove(void);
void Game_State15_ScrollToTileThenReturnToCursor(void);
void Game_State15_ScrollView(void);
void Game_State15_WaitScrollThenReturnToCursor(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game state 15 (the player's turn on the map) runs one handler per
 * sub-state, kept in unk_1c: 0 is the free cursor, 1 picks the move
 * destination (UnitMoveSelect_Update). The others are still unnamed. */
void Game_State15_RunStep(void) {
  switch (GAME->unk_1c) {
    case 0:
      Game_State15_FreeCursor();
      break;
    case 1:
      UnitMoveSelect_Update();
      break;
    case 2:
      Game_State15_PickUnitToDelete();
      break;
    case 3:
      Game_State15_OpenMenuAtDestination();
      break;
    case 4:
      Game_State15_AwaitUnitCommand();
      break;
    case 5:
      Game_State15_EndBlockedMove();
      break;
    case 6:
      Game_State15_ScrollToTileThenReturnToCursor();
      break;
    case 7:
      Game_State15_ScrollView();
      break;
    case 8:
      Game_State15_WaitScrollThenReturnToCursor();
      break;
  }
}
