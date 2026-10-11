#include "types.h"
#include "game/Unk_022a8a10.h"

extern Unk_022a8a10 data_022a8a10;
extern u8 data_022a89fc[];

void SND_StopTimer(int arg0, int arg1, u32 alarmMask, int arg3);
int SND_GetCurrentCommandTag(void);
void SND_WaitForCommandProc(int arg0);
void SND_FlushCommand(int arg0);
int OS_ReceiveMessage(void *arg0, int arg1, int arg2);
void func_02104f90(int arg0);
void Sound_UnlockChannels(u32 channelMask);
void SoundAlarm_Free(int alarm);
void func_0210fb4c(int arg0, int arg1, int arg2, int arg3);

/* Stops the output and gives back its alarm, the locked channels and
 * what it set up. */
void SoundOutput_Close(void)
{
    Unk_022a8a10 *state = &data_022a8a10;
    int hasAlarm;

    if (state->running == 0) {
        return;
    }
    hasAlarm = state->alarm >= 0;
    SND_StopTimer(state->unk_20, state->unk_24, hasAlarm ? 1 << state->alarm : 0, 0);
    if (hasAlarm) {
        int saved = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        SND_WaitForCommandProc(saved);
        while (OS_ReceiveMessage(data_022a89fc, 0, 0) != 0) {
        }
    }
    if (state->unk_24 != 0) {
        func_02104f90(state->unk_24);
    }
    if (state->unk_1c != 0) {
        Sound_UnlockChannels(state->unk_1c);
    }
    if (hasAlarm) {
        SoundAlarm_Free(state->alarm);
    }
    if (state->unk_04 == 1) {
        func_0210fb4c(0, 0, 0, 0);
    }
    state->running = 0;
}
