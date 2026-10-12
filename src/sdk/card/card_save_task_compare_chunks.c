/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
void DC_WaitWriteBufferEmpty(void);
BOOL CARDi_Request(CARDiCommon *common, u32 command, int tries);   /* sends one command to the backup chip */
void CardSave_FinishTaskAndNotify(CARDiCommon *common);

/* Card thread task: compares backup memory with the data at src, one page buffer at a time. */
void CardSave_TaskCompareChunks(CARDiCommon *common)
{
    u32 chunk;

    do {
        chunk = common->len;
        if (chunk > GAMECARD_BOUNCE_BUF_BYTES) {
            chunk = GAMECARD_BOUNCE_BUF_BYTES;
        }
        Mem_CopyBytes((void *)common->src, common->pageBuffer, chunk);
        DC_FlushRange(common->pageBuffer, chunk);
        DC_WaitWriteBufferEmpty();
        common->cmd->src = (u32)common->pageBuffer;
        common->cmd->dst = common->dst;
        common->cmd->len = chunk;
        if (!CARDi_Request(common, 9, 1)) {
            break;
        }
        common->src += chunk;
        common->dst += chunk;
        common->len -= chunk;
    } while (common->len != 0);
    CardSave_FinishTaskAndNotify(common);
}
