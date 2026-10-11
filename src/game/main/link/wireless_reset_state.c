#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"

extern u8 data_0229c8f8[];

void OwnerInfo_CopyNickname(void *buffer);
void Wireless_ClearPlayerName(u8 aid);
void Wireless_ClearParents(void);

/* Resets all wireless state: every field of Unk_0215fe84, the stored player
 * names and the list of scanned parents. Link_ResetState calls it first. */
void Wireless_ResetState(void)
{
    int aid;

    data_0215fe84->parentCount = 0;
    data_0215fe84->unk_18 = 1;
    data_0215fe84->childrenChanged = 0;
    data_0215fe84->dataSharingActive = 0;
    data_0215fe84->childAids = 0;
    data_0215fe84->childCount = 0;
    data_0215fe84->unk_20 = 0;
    data_0215fe84->unk_24 = 0;
    data_0215fe84->unk_28 = 0;
    data_0215fe84->unk_2c = 0;
    data_0215fe84->scanning = 0;
    data_0215fe84->unk_30 = 0;
    data_0215fe84->channel = 1;
    data_0215fe84->beaconLostCount = 0;
    OwnerInfo_CopyNickname(&data_0229c910);
    OwnerInfo_CopyNickname(data_0229c8f8);
    for (aid = 0; aid < 8; aid++) {
        Wireless_ClearPlayerName(aid);
    }
    Wireless_ClearParents();
}
