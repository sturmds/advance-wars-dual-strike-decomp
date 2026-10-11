#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd gLinkMapRecordSyncScript[];

/* Starts the design map record swap as a child of parent, in link play only. */
void LinkMapRecordSync_Start(Task *parent)
{
    if (Link_IsActive() == 1) {
        Task_CreateChild(gLinkMapRecordSyncScript, parent);
    }
}
