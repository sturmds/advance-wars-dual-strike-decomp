#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8ab8;         /* refill requests waiting for the sound thread */
extern u8 data_022a8ac4[];             /* message queue the sound thread waits on */

SoundStreamRequest *SoundStreamRequest_Alloc(void);
void SoundStreamRequest_Free(SoundStreamRequest *request);
void func_0210f52c(void *dest, int value, u32 size);
int func_0210ddcc(void *queue, void *message, int flags);

/* Stream driver callback of every SoundStream, with the stream as arg: queues
 * a refill request for the sound thread and wakes it. With two or more
 * requests already waiting for the stream, the oldest is removed and its
 * blocks are filled with silence. */
void SoundStream_QueueRefill(int status, int channelCount, void *blocks[], u32 blockSize, int format, SoundStream *stream)
{
    SoundStreamRequest *request;
    int i;

    if (stream->pendingRequests >= 2) {
        request = LinkList_GetNext(&data_022a8ab8, NULL);
        while (request != NULL) {
            if (request->stream == stream) {
                break;
            }
            request = LinkList_GetNext(&data_022a8ab8, request);
        }
        for (i = 0; i < request->channelCount; i++) {
            func_0210f52c(request->blocks[i], 0, request->blockSize);
        }
        LinkList_Remove(&data_022a8ab8, request);
        stream->pendingRequests--;
        SoundStreamRequest_Free(request);
    }

    request = SoundStreamRequest_Alloc();
    request->stream = stream;
    request->status = status;
    request->channelCount = channelCount;
    for (i = 0; i < channelCount; i++) {
        request->blocks[i] = blocks[i];
    }
    request->blockSize = blockSize;
    stream->pendingRequests++;
    LinkList_Append(&data_022a8ab8, request);
    func_0210ddcc(data_022a8ac4, NULL, 0);
}
