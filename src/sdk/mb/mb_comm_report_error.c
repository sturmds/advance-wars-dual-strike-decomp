#include "types.h"
#include "sdk/mb.h"

/* Tells the comm callback about an error on a child's connection. */
void MbComm_ReportError(u32 aid, u16 errorCode) {
  u16 code =
      errorCode; /* needed to match: taking the parameter's address spills r0 to r3 */

  MbComm_NotifyState(aid, DLPLAY_CHILD_FAULT, &code);
}
