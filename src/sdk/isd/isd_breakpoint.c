#include "types.h"

#ifdef __MWERKS__
/*
 * Executes an undefined instruction so an attached debugger stops here; the assertion
 * printer calls it. The assembler has no udf mnemonic: 0xe7ffffff encodes "udf #0xffff".
 */
asm void Isd_Breakpoint(void)
{
    dcd 0xe7ffffff
    bx lr
}
#else
#error "Isd_Breakpoint: an undefined instruction that stops an attached debugger has no portable C equivalent"
#endif
