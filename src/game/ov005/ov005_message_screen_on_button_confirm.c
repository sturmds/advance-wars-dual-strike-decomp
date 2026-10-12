#include "types.h"
#include "game/Unk_ov005_0236bb8c.h"

extern Unk_ov005_0236bb8c* data_ov005_0236bb8c;

/* onConfirm of the message screen button (gOv005MessageScreenButton). */
void Ov005MessageScreen_OnButtonConfirm(void) {
  data_ov005_0236bb8c->confirmed = 1;
}
