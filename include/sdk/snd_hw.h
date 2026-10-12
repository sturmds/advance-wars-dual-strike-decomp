#ifndef SDK_SND_HW_H
#define SDK_SND_HW_H

#include "types.h"

/* Sound hardware registers the ARM7 driver writes. */

/* High byte of SOUNDCNT: bits 0-1 left output, 2-3 right output, 4 and 5 channel 1 and 3 direct output, 7 master enable. */
#define AUDIO_MIXER_ROUTE_BYTE (*(u8 *)0x4000501)
#define AUDIO_MIXER_MASTER_ON 0x80

/* POWCNT2: bit 0 powers the speakers and sound circuit. */
#define AUDIO_SPEAKER_POWER_REG (*(u16 *)0x4000304)

#endif
