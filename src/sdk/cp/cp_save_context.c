#include "sdk/cp.h"

/* Reads the divider and square root inputs and modes into context. */
void CP_SaveContext(CPContext *context)
{
    context->div_numer = IO_DIVIDER_DIVIDEND;
    context->div_denom = REG_DIV_DENOM;
    context->div_mode = REG_DIVCNT & 3;
    context->sqrt = IO_ROOT_UNIT_INPUT;
    context->sqrt_mode = IO_ROOT_UNIT_CTRL & 1;
}
