#include "sdk/os_arm7.h"
#include "sdk/snd.h"
#include "sdk/snd_seq.h"
#include "bss_run.h"

/* Message queue init; the definition types the buffer as int. */

#define SOUND_THREAD_MSG_SLOTS 8

BOOL gSndArm7Initialized;
#pragma section BSS_RUN1 begin
u32 data_arm7_0380806c;
u32 data_arm7_03808068;
int gSndPrintVarEnabled;
#pragma section BSS_RUN1 end
OSMessageQueue data_arm7_03807b90; /* messages to the driver thread */
#pragma section BSS_RUN1 begin
SndSeqCache data_arm7_03808074;
#pragma section BSS_RUN1 end
/* buffer of data_arm7_03807b90 */
static OSMessage data_arm7_03807ba4[SOUND_THREAD_MSG_SLOTS];
OSAlarm data_arm7_03807bc4;  /* the interval alarm */
OSThread data_arm7_03807bf0; /* the driver thread */
/* 0x400 bytes no code names, before data_arm7_03808068 */
u8 data_arm7_03807c68[0x400];

void SND_InitIntervalTimer(void) {
  MsgQueue_SetUpRing_Arm7(&data_arm7_03807b90, data_arm7_03807ba4,
                          SOUND_THREAD_MSG_SLOTS);
  Alarm_Clear_Arm7(&data_arm7_03807bc4);
}
