#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* MenuButton callback of the A touch button. */
void Ov005SoundRoom_OnATouched(void) { data_ov005_0236bbac->aTouched = 1; }
