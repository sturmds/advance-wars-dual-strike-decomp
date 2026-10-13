#include "types.h"
#include "game/sound_stream.h"

extern u8 data_022a8aa8[]; /* message buffer of the queue, one message */
extern u8 data_022a8ac4[]; /* message queue the sound thread waits on */

void OS_InitMessageQueue(void* queue, void* messages, int count);
int OS_ReceiveMessage(void* queue, void* message, int flags);
void OS_LockMutex(void* mutex);
void OS_UnlockMutex(void* mutex);
SoundStreamRequest* SoundStreamRequest_PopPending(void);
void SoundStream_FillBlocks(SoundStreamRequest* request);
void SoundStreamRequest_Free(SoundStreamRequest* request);

/* Entry of the sound stream thread. Sleeps on a message queue; each message
 * means refill requests are waiting, and it handles them all before it
 * sleeps again. */
void SoundStream_ThreadMain(void) {
  SoundStreamRequest* request;
  u32 message[2];

  OS_InitMessageQueue(data_022a8ac4, data_022a8aa8, 1);
  while (TRUE) {
    OS_ReceiveMessage(data_022a8ac4, message, 1);
    while (TRUE) {
      OS_LockMutex(data_022a8ad8);
      request = SoundStreamRequest_PopPending();
      if (request == NULL) {
        OS_UnlockMutex(data_022a8ad8);
        break;
      }
      SoundStream_FillBlocks(request);
      SoundStreamRequest_Free(request);
      OS_UnlockMutex(data_022a8ad8);
    }
  }
}
