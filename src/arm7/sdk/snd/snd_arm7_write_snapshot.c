#include "sdk/snd.h"
#include "sdk/mi.h"

int func_arm7_037fe73c(int channel);

void SndArm7_WriteSnapshot(SndArm7Snapshot *snapshot)
{
    int i;

    Mem_CopyWords_Arm7(&data_arm7_03808090, snapshot, sizeof(SndArm7Work));
    snapshot->driver = &data_arm7_03808090;
    for (i = 0; i < 16; i++) {
        snapshot->channelControl[i] = func_arm7_037fe73c(i);
    }
    snapshot->unk_11C4 = SndExChannel_GetLockedChannels(0);
}
