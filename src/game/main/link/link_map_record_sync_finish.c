#include "types.h"
#include "game/link_map_record_sync_task.h"

void Mem_Free(void* block);

/* Step of the gLinkMapRecordSyncScript script after the swap: frees the send buffer. */
void LinkMapRecordSync_Finish(LinkMapRecordSyncTask* task) {
  Mem_Free(task->sendBuffer);
}
