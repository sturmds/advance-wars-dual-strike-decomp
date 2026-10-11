#include "sdk/cp.h"

/* Writes a saved divider and square root state back to the hardware. */
void CPi_RestoreContext(const CPContext *context)
{
    IO_DIVIDER_DIVIDEND = context->div_numer;
    REG_DIV_DENOM = context->div_denom;
    REG_DIVCNT = context->div_mode;
    IO_ROOT_UNIT_INPUT = context->sqrt;
    IO_ROOT_UNIT_CTRL = context->sqrt_mode;
}
