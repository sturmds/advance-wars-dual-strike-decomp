#include "types.h"
#include "game/co_select_screen.h"

void PlayerInfo_StartTaskFor(int, int, int);

void func_02042e9c(int a0) {
  CoSelectScreen* t0 = data_0229c73c;
  PlayerInfo_StartTaskFor(a0, t0->cursorColumn, t0->cursorRow);
}
