/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
BOOL CARDi_Request(CARDiCommon *common, u32 command, int tries);   /* sends one command to the backup chip */
void CardSave_FinishTaskAndNotify(CARDiCommon *common);

/* Card thread task: reads backup memory at src into dst, one page buffer at a time. */
void CardSave_TaskReadChunks(CARDiCommon *common)
{
    u32 chunk;

    do {
        chunk = common->len;
        if (chunk > GAMECARD_BOUNCE_BUF_BYTES) {
            chunk = GAMECARD_BOUNCE_BUF_BYTES;
        }
        DC_InvalidateRange(common->pageBuffer, GAMECARD_BOUNCE_BUF_BYTES);
        common->cmd->unk_0C = common->src;
        common->cmd->unk_10 = (u32)common->pageBuffer;
        common->cmd->unk_14 = chunk;
        if (!CARDi_Request(common, 6, 1)) {
            break;
        }
        Mem_CopyBytes(common->pageBuffer, (void *)common->dst, chunk);
        common->src += chunk;
        common->dst += chunk;
        common->len -= chunk;
    } while (common->len != 0);
    CardSave_FinishTaskAndNotify(common);
}
