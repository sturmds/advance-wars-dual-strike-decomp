#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02031a14.h"
#include "game/win_lose.h"

int CoExp_IsSingleColumn(void);

/* Starts the func_02031a14 task for one of six cells in a 2-column,
 * 3-row grid (x 40 or 152, y 17, 73 or 129), passing 31 - item. The grid
 * moves right and down in the war room (down only unless CoExp_IsSingleColumn
 * says 1) and on campaign maps up to 231; otherwise it moves down when the
 * win/lose screen's side A has one or two players. */
void func_020318ac(int item) {
  Unk_02080440_48* state;
  int arg; /* needed to match: declared before dx and dy */
  int dx;
  int dy;

  state = data_027c027c->unk_48[data_027c0284];
  dx = 0;
  dy = dx;
  arg = 31 - item;

  if (state->gameMode == GAME_MODE_WAR_ROOM) {
    dy = 40;
    if (CoExp_IsSingleColumn() == 1) {
      dx = 56;
    }
  } else if (state->gameMode == GAME_MODE_CAMPAIGN &&
             state->unk_1c4->mapId <= 231) {
    dy = dx = 56;
  } else {
    switch (data_022a7a88.countA) {
      case 1:
        dy = 56;
        break;
      case 2:
        dy = 28;
        break;
    }
  }
  switch (item) {
    case 0:
      func_02031a14(dx + 40, dy + 17, arg);
      return;
    case 1:
      func_02031a14(dx + 152, dy + 17, arg);
      return;
    case 2:
      func_02031a14(dx + 40, dy + 73, arg);
      return;
    case 3:
      func_02031a14(dx + 152, dy + 73, arg);
      return;
    case 4:
      func_02031a14(dx + 40, dy + 129, arg);
      return;
    case 5:
      func_02031a14(dx + 152, dy + 129, arg);
      return;
  }
}
