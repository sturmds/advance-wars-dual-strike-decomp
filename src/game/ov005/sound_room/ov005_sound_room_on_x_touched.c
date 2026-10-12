#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* MenuButton callback of the X touch button. */
void Ov005SoundRoom_OnXTouched(void) { data_ov005_0236bbac->xTouched = 1; }
