#include "types.h"
#include "sdk/mb.h"

void func_0211a3c4(u32 aid, u32 state, void *arg);

/* Tells the comm callback about an error on a child's connection. */
void MbComm_ReportError(u32 aid, u16 errorCode)
{
    u16 code = errorCode; /* needed to match: taking the parameter's address spills r0 to r3 */

    func_0211a3c4(aid, DLPLAY_CHILD_FAULT, &code);
}
