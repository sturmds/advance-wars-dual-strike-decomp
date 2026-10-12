#include "sdk/snd_hw.h"

/* Bit 7 of a capture control register: set while that capture runs. */
#define AUDIO_RECORD_RUNNING 0x80

/* capture: 0 or 1. */
BOOL SND_IsCaptureActive(int capture)
{
    return ((&AUDIO_RECORD0_CTRL)[capture] & AUDIO_RECORD_RUNNING) ? TRUE : FALSE;
}
