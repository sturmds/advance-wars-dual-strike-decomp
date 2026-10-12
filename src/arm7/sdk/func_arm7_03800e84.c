#include "types.h"

void SndTrack_ReleaseChannels(int, int, int);
int SndTrack_FreeChannels(int);

int func_arm7_03800e84(int a0, int a1)
{
    SndTrack_ReleaseChannels(a0, a1, -1);
    return SndTrack_FreeChannels(a0);
}
