#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void func_ov002_0235d514(int index) {
  /* Needed to match: the original adds the field offset at run time. */
  *((u8*)&gMenuState + index + (u32) & ((MenuState*)0)->battle.unk_1000) = 1;
}
