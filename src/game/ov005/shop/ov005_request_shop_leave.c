#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Touch button: leaves the shop like B. */
void Ov005_RequestShopLeave(void) { data_ov005_0236bbb0->leaveRequested = 1; }
