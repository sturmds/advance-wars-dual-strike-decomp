#include "types.h"
#include "game/Unk_022a8a10.h"

extern Unk_022a8a10 data_022a8a10;
extern u8 data_022a89fc[];

void func_0210fd58(int arg0, int arg1, u32 channelMask, int arg3);
int func_02110150(void);
void func_02110220(int arg0);
void func_021102b0(int arg0);
int func_0210dd20(void *arg0, int arg1, int arg2);
void func_02104f90(int arg0);
void Sound_UnlockChannels(u32 channelMask);
void SoundAlarm_Free(int channel);
void func_0210fb4c(int arg0, int arg1, int arg2, int arg3);

/* Stops the output and gives back its hardware channel, the locked channels and
 * what it set up. */
void SoundOutput_Close(void)
{
    Unk_022a8a10 *state = &data_022a8a10;
    int hasChannel;

    if (state->running == 0) {
        return;
    }
    hasChannel = state->channel >= 0;
    func_0210fd58(state->unk_20, state->unk_24, hasChannel ? 1 << state->channel : 0, 0);
    if (hasChannel) {
        int saved = func_02110150();
        func_021102b0(1);
        func_02110220(saved);
        while (func_0210dd20(data_022a89fc, 0, 0) != 0) {
        }
    }
    if (state->unk_24 != 0) {
        func_02104f90(state->unk_24);
    }
    if (state->unk_1c != 0) {
        Sound_UnlockChannels(state->unk_1c);
    }
    if (hasChannel) {
        SoundAlarm_Free(state->channel);
    }
    if (state->unk_04 == 1) {
        func_0210fb4c(0, 0, 0, 0);
    }
    state->running = 0;
}
