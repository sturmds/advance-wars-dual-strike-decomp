/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/ctrdg.h"

/* needed to match: the original reloads the flag at each use */
#define FLAG(c) (*(volatile u32 *)&(c)->flag)

void DC_FlushRange(const void *start, u32 size);
void DC_WaitWriteBufferEmpty(void);
void OS_SleepThread(OSThreadQueue *queue);
int func_02110f88(int tag, u32 word, int err);
int func_0211103c(int tag, int arg);

/*
 * Sends one request to the other CPU and sleeps until it answers. A request of
 * GAMECARD_REQ_HANDSHAKE also sends the command block's address. Sends again, up to
 * retries times, while the answer asks for it. TRUE when the result is success.
 */
BOOL CARDi_Request(CARDiCommon *common, u32 request, int retries)
{
    int state;
    int result;

    if (!(FLAG(common) & GAMECARD_FLAG_PEER_GREETED)) {
        FLAG(common) |= GAMECARD_FLAG_PEER_GREETED;
        if (!func_0211103c(FIFO_TAG_CARD_SERVICE, 1)) {
            do {
                OS_SpinWait(100);
            } while (!func_0211103c(FIFO_TAG_CARD_SERVICE, 1));
        }
        CARDi_Request(common, GAMECARD_REQ_HANDSHAKE, 1);
    }
    DC_FlushRange(common->cmd, 64);
    DC_WaitWriteBufferEmpty();
    do {
        common->unk_04 = request;
        FLAG(common) |= GAMECARD_FLAG_AWAIT_PEER_REPLY;
        while (func_02110f88(FIFO_TAG_CARD_SERVICE, request, 1) < 0) {
        }
        if (request == GAMECARD_REQ_HANDSHAKE) {
            u32 cmdAddress = (u32)common->cmd;

            while (func_02110f88(FIFO_TAG_CARD_SERVICE, cmdAddress, 1) < 0) {
            }
        }
        state = OS_DisableInterrupts();
        while (FLAG(common) & GAMECARD_FLAG_AWAIT_PEER_REPLY) {
            OS_SleepThread(NULL);
        }
        OS_RestoreInterrupts(state);
        result = common->cmd->result;
    } while (result == GAMECARD_STATUS_RESEND && --retries > 0);
    return result == CARD_RESULT_SUCCESS;
}
