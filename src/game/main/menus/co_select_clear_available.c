#include "types.h"
#include "game/co_select_screen.h"

/* One bit per CO id: the COs the CO select screen offers. */

void CoSelect_ClearAvailable(void) { data_0229c740 = 0; }
