#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

/* Indication callback registered by func_0207930c: on state 16 clears unk_36. */
void Wireless_OnIndication(RadioNotice *ind)
{
    if (ind->state == 16) {
        data_0215fe84->beaconLostCount = 0;
    }
}
