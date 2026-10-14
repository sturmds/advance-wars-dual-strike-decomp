#include "types.h"
#include "sdk/msl.h"

static FpTrapHandler gFpTrapHandlers[FP_TRAP_HANDLER_COUNT];

FpTrapHandler* FpTrap_GetHandlers(void) { return gFpTrapHandlers; }
