#include "types.h"

/* One bit per CO id: the COs the CO select screen offers. */
extern u32 data_0229c740;

void CoSelect_AddAvailable(int coId) { data_0229c740 |= 1 << coId; }
