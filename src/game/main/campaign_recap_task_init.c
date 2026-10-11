#include "types.h"
#include "game/campaign_recap.h"

extern CampaignRecap *data_022a7ea4;

void *Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);

/* Allocates the work block and the 28 mission entries, both zeroed. */
void CampaignRecapTask_Init(CampaignRecapTask *task)
{
    task->work = Mem_Alloc(sizeof(CampaignRecap));
    data_022a7ea4 = task->work;
    Mem_Zero32(0, data_022a7ea4, sizeof(CampaignRecap));
    data_022a7ea4->count = 28;
    data_022a7ea4->unk_7c = Mem_Alloc(28 * sizeof(CampaignRecapEntry));
    Mem_Zero32(0, data_022a7ea4->unk_7c, 28 * sizeof(CampaignRecapEntry));
}
