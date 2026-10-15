#include "types.h"
#include "game/Unk_02080440.h"
#include "game/visual_mode.h"

u8 func_020c019c(void);
int func_020c0144(void);

BOOL func_020bff48(void) {
  if (func_020c019c() == 1) {
    return TRUE;
  }
  if (func_020c0144() == 0 && MAP_STATE->visualMode == VISUAL_C) {
    return FALSE;
  }
  return TRUE;
}
