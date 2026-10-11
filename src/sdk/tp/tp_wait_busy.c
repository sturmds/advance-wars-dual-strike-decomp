#include "sdk/tp.h"

/* Spins until the ARM7 has answered every request in commandFlags. */
void TP_WaitBusy(u32 commandFlags)
{
    while (tpState.command_flg & commandFlags) {
    }
}
