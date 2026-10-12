#ifndef SDK_SND_DRV_H
#define SDK_SND_DRV_H

#include "sdk/os_arm7.h"

/*
 * Thread of the ARM7 sound driver and the alarm that wakes it every 2728
 * ticks (about 5.2 ms). Kept apart from sdk/snd.h, which uses the ARM9 OS
 * types.
 */

/* Messages the driver thread waits for. */
enum {
    SOUND_THREAD_MSG_TICK = 1, /* the interval alarm fired: run sequences and envelopes */
    SOUND_THREAD_MSG_NEW_REQUESTS = 2    /* the ARM9 sent commands */
};

extern OSMessageQueue data_arm7_03807b90; /* messages to the driver thread */
extern OSAlarm data_arm7_03807bc4;        /* the interval alarm */

void SND_InitIntervalTimer(void);
void SND_StartIntervalTimer(void);
OSMessage SND_WaitForIntervalTimer(void);

#endif
