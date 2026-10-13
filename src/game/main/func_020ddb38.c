#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern UnitAction data_0229d8c8;

int Weather_PickNext(void); /* weather for the next turn */
void Player_RefreshUnits(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020ddb38(void) {
  GAME->unk_1c4->nextWeather = Weather_PickNext();
  data_0229d8c8.rngState = gRngState;
  if (gLinkPlay == 0) {
    GAME->players[GAME->currentPlayer].savedCursorX = GAME->cursorX;
    GAME->players[GAME->currentPlayer].savedCursorY = GAME->cursorY;
  }
  GAME->unk_18 = 1;
  Player_RefreshUnits();
}
