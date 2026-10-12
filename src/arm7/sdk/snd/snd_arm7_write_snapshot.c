#include "sdk/snd.h"

void func_arm7_037fde70(const void *src, void *dest, int size);
int func_arm7_037fe73c(int channel);
u32 func_arm7_037ff470(int arg);

void SndArm7_WriteSnapshot(SndArm7Snapshot *snapshot)
{
    int i;

    func_arm7_037fde70(&data_arm7_03808090, snapshot, sizeof(SndArm7Work));
    snapshot->driver = &data_arm7_03808090;
    for (i = 0; i < 16; i++) {
        snapshot->channelControl[i] = func_arm7_037fe73c(i);
    }
    snapshot->unk_11C4 = func_arm7_037ff470(0);
}
