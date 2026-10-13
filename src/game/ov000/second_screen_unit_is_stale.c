#include "types.h"
#include "game/second_screen.h"

/* True when the unit requested for the second screen (data_ov000_02350530,
 * 0235054c, 0235053c, set by SecondScreenTask_Create and SecondScreen_ShowCursorInfo)
 * differs from the one data_ov000_02350538, 02350534 and 02350544 hold. */
BOOL SecondScreenUnit_IsStale(void) {
  if (data_ov000_02350530 != data_ov000_02350538 ||
      data_ov000_0235054c != data_ov000_02350534 ||
      data_ov000_0235053c != data_ov000_02350544) {
    return TRUE;
  }
  return FALSE;
}
