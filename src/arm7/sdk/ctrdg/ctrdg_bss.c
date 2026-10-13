#include "types.h"
#include "sdk/ctrdg.h"

u32 gCtrdgPollStopped; /* set when no cartridge was there at the first check */
CTRDGWork data_arm7_0380afc0;
u32 data_arm7_0380afd0;
u32 gCtrdgPulledOut; /* set once the cartridge was found pulled out */
u32 gCtrdgHeaderReceived;
GbaSlotInitWord gCtrdgInitMessage;
