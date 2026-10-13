#include "types.h"
#include "game/co_select_screen.h"

u32 data_0229c740;

BOOL CoSelect_IsCoAvailable(u8 coId) {
  if (coId >= 28) {
    return FALSE;
  }
  return (data_0229c740 & (1 << coId)) ? TRUE : FALSE;
}
