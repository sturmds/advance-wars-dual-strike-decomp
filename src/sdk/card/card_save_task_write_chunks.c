/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
void DC_WaitWriteBufferEmpty(void);
BOOL CARDi_Request(CARDiCommon *common, u32 command, int tries);   /* sends one command to the backup chip */
void CardSave_FinishTaskAndNotify(CARDiCommon *common);

/* Card thread task: writes the data at src to backup memory, one page buffer at a time. */
void CardSave_TaskWriteChunks(CARDiCommon *common)
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
        common->cmd->unk_0C = (u32)common->pageBuffer;
        common->cmd->unk_10 = common->dst;
        common->cmd->unk_14 = chunk;
        if (!CARDi_Request(common, 7, 10)) {
            break;
        }
        common->src += chunk;
        common->dst += chunk;
        common->len -= chunk;
    } while (common->len != 0);
    CardSave_FinishTaskAndNotify(common);
}
