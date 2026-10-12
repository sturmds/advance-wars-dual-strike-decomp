#include "types.h"

/* One bit per CO id: the COs the CO select screen offers. */
extern u32 data_0229c740;

void CoSelect_ClearAvailable(void) { data_0229c740 = 0; }
