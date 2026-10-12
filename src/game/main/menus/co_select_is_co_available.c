#include "types.h"

/* One bit per CO id: the COs the CO select screen offers. */
extern u32 data_0229c740;

BOOL CoSelect_IsCoAvailable(u8 coId) {
  if (coId >= 28) {
    return FALSE;
  }
  return (data_0229c740 & (1 << coId)) ? TRUE : FALSE;
}
