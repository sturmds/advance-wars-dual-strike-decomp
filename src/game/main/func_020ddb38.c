#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern u32 gLinkPlay;
extern UnitAction data_0229d8c8;

int Weather_PickNext(void); /* weather for the next turn */
void Player_RefreshUnits(void);

void func_020ddb38(void) {
  MAP_STATE->unk_1c4->nextWeather = Weather_PickNext();
  data_0229d8c8.rngState = gRngState;
  if (gLinkPlay == 0) {
    MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorX =
        MAP_STATE->cursorX;
    MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorY =
        MAP_STATE->cursorY;
  }
  MAP_STATE->unk_18 = 1;
  Player_RefreshUnits();
}
