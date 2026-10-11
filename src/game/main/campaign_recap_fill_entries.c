#include "types.h"
#include "game/campaign_recap.h"
#include "game/Unk_02290118.h"

/* Copies the saved campaign records into the recap entries: map number,
 * days (unk_04) and score (unk_14). */
void CampaignRecap_FillEntries(CampaignRecap *work)
{
    int i;

    for (i = 0; i < (int)work->count; i++) {
        work->unk_7c[i].unk_00 = data_02290118.records[i].mapIndex;
        work->unk_7c[i].unk_04 = gCampaignMapRecords[i].days;
        work->unk_7c[i].unk_14 = gCampaignMapRecords[i].score;
    }
}
