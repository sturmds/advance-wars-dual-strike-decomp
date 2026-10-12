#include "sdk/wl.h"

void WifiStats_Accumulate(void)
{
    WlCounters *counters = (WlCounters *)WIFI_DRIVER_STATE->counters;
    volatile u16 *stat = IO_WIRELESS_TALLY_BYTES;
    u16 pair;

    pair = stat[0];
    counters->unk_50 += pair & 0xff;
    pair = stat[1];
    counters->unk_4c += pair >> 8;
    counters->unk_5c += pair & 0xff;
    pair = stat[2];
    counters->unk_58 += pair >> 8;
    counters->unk_54 += pair & 0xff;
    pair = stat[3];
    counters->unk_60 += pair >> 8;
    counters->unk_38 += pair & 0xff;
    pair = stat[4];
    counters->unk_48 += pair & 0xff;
    pair = stat[5];
    counters->unk_20 += pair & 0xff;
    pair = stat[6];
    counters->unk_30 += pair >> 8;
    counters->unk_44 += pair & 0xff;
    pair = stat[7];
    counters->unk_3c += pair & 0xff;
    counters->unk_40 += pair >> 8;
    pair = stat[8];
    counters->unk_0c += pair & 0xff;

    pair = stat[16];
    counters->cmdStat[0] += pair >> 8;
    pair = stat[17];
    counters->cmdStat[1] += pair & 0xff;
    counters->cmdStat[2] += pair >> 8;
    pair = stat[18];
    counters->cmdStat[3] += pair & 0xff;
    counters->cmdStat[4] += pair >> 8;
    pair = stat[19];
    counters->cmdStat[5] += pair & 0xff;
    counters->cmdStat[6] += pair >> 8;
    pair = stat[20];
    counters->cmdStat[7] += pair & 0xff;
    counters->cmdStat[8] += pair >> 8;
    pair = stat[21];
    counters->cmdStat[9] += pair & 0xff;
    counters->cmdStat[10] += pair >> 8;
    pair = stat[22];
    counters->cmdStat[11] += pair & 0xff;
    counters->cmdStat[12] += pair >> 8;
    pair = stat[23];
    counters->cmdStat[13] += pair & 0xff;
    counters->cmdStat[14] += pair >> 8;
}
