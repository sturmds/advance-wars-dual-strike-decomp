#include "types.h"
#include "game/campaign_recap.h"

void Mem_Free(void* block);

/* Frees the entry list and the work block. */
void CampaignRecapTask_Free(CampaignRecapTask* task) {
  CampaignRecap* work = task->work;

  Mem_Free(work->unk_7c);
  Mem_Free(work);
}
