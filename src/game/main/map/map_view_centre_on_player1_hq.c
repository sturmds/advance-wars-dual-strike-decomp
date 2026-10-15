#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(int player);
BOOL Player_HasHq(int player);
void func_020f5898(int x, int y);

/* At battle start: when player 1 is in the game and has an HQ, puts the view
 * on the HQ tile (func_020f5898 centres it through func_020f53e8). */
void MapView_CentreOnPlayer1Hq(void) {
  if (!Player_IsInGame(1)) {
    return;
  }
  if (!Player_HasHq(1)) {
    return;
  }
  func_020f5898(MAP_STATE->players[1].hqX & 0x7f,
                MAP_STATE->players[1].hqY & 0x7f);
}
