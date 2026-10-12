#include "types.h"
#include "sdk/wl.h"

/* Spins while carrier sense is up or the RF unit is in state 6 or 5 (GBATEK lists 5 as transmitting). */
void WifiRf_AwaitQuietAir(void)
{
    do {
        while (REG_W_RF_PINS & 1) {
        }
    } while (REG_W_RF_STATUS == 6 || REG_W_RF_STATUS == 5);
}
