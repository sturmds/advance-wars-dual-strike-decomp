#include "types.h"
#include "sdk/snd_seq.h"
#include "text_data.h"

/* The values come from the ROM at build time (config/AWRE/data_inc.txt).
 * mwcc emits these tables smallest first, which is also their order in the
 * ROM. */
#pragma section TEXT_DATA begin

/* First quarter of a sine period of 128 steps, plus the peak
 * (SndDrv_SineWave). */
s8 gSndSineQuarterTable[33] = {
#include "data/arm7/gSndSineQuarterTable.inc.c"
};

/* Attenuation in tenths of a decibel (-723 to 0) for each volume 0-127. */
s16 gSndVolumeDecibels[128] = {
#include "data/arm7/gSndVolumeDecibels.inc.c"
};

#pragma section TEXT_DATA end
