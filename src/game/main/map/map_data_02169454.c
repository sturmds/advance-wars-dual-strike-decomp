#include "types.h"
#include "game/keypad.h"

/* per d-pad bit pattern (right, left, up, down): x step, y step */
s16 data_02169454[DPAD_PATTERN_COUNT][2] = {
#include "data/main/data_02169454.inc.c"
};
