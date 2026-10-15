#include "types.h"
#include "game/Unk_02080440.h"

/* Makes state the current screen state: data_027c0288 holds one per
 * screen, and most code reads the current one through data_027c027c. */
void Screen_SetCurrent(Unk_02080440* state) { data_027c027c = state; }
