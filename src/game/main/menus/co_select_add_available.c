#include "types.h"
#include "game/co_select_screen.h"

/* One bit per CO id: the COs the CO select screen offers. */

void CoSelect_AddAvailable(int coId) { data_0229c740 |= 1 << coId; }
