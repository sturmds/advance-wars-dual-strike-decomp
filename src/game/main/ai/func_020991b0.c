#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the weather is not snow; in snow, TRUE only for mode 2. Nothing
 * calls it. */
BOOL func_020991b0(int mode) {
  if (STATE->unk_1c4->weather != WEATHER_SNOW) {
    return TRUE;
  }
  return mode == 2;
}
