#include "types.h"
#include "sdk/snd_hw.h"
#include "text_data.h"

/* The values come from the ROM at build time (config/AWRE/data_inc.txt).
 * mwcc emits these tables smallest first, which is also their order in the
 * ROM. */
#pragma section TEXT_DATA begin

/* Right shift the hardware applies for each volume shift setting: 0, 1, 2,
 * 4 bits (SndExChannel_CompareVolume). */
u8 gSndVolumeShiftBits[] = {
#include "data/arm7/gSndVolumeShiftBits.inc.c"
};

/* Hardware channel numbers in the order the allocator tries them. */
u8 gSndExChannelAllocOrder[AUDIO_CH_TOTAL] = {
#include "data/arm7/gSndExChannelAllocOrder.inc.c"
};

/* Attack multipliers for the settings 109-127, fastest first
 * (SndExChannel_SetAttack). */
u8 gSndAttackTable[] = {
#include "data/arm7/gSndAttackTable.inc.c"
};

#pragma section TEXT_DATA end
