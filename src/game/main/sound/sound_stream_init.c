#include "types.h"
#include "game/link_list.h"
#include "game/sound_stream.h"
#include "sdk/fs.h"
#include "sdk/os.h"

#define SOUND_STREAM_REQUEST_COUNT 8
#define SOUND_STREAM_READ_BUFFER_SIZE 0x200
#define SOUND_STREAM_THREAD_STACK_SIZE 0x400

u8* data_022a8aa4;
OSMessage data_022a8aa8[SOUND_STREAM_MESSAGE_COUNT];
/* TRUE once the stream system is set up */
static BOOL gSoundStreamReady;
LinkList data_022a8ab8;
LinkList data_022a8aac;
OSMutex data_022a8ad8;
OSMessageQueue data_022a8ac4;
static OSThread gSoundStreamThread;
static SoundStreamRequest gSoundStreamRequests[SOUND_STREAM_REQUEST_COUNT];
/* The ADPCM read buffer, then the thread's stack, which grows down from its
 * end. */
static u8 data_022a8d00[SOUND_STREAM_READ_BUFFER_SIZE +
                        SOUND_STREAM_THREAD_STACK_SIZE];

BOOL SoundStream_AllocBuffers(void* heap);
void func_0210e0c0(void* mutex);
void OS_CreateThread(void* thread, void (*entry)(void), void* arg,
                     void* stackTop, u32 stackSize, u32 priority);
void OS_WakeupThreadDirect(void* thread);
void SoundStrm_Init(SoundStrm* strm);
void SoundStream_ThreadMain(void);

/* Sets up the four stream slots, the refill request pool and the sound stream
 * thread, then gives the slots block memory from heap. Called again, it only
 * gives the slots new block memory. NitroSystem's NNS_SndArcStrmInit. */
void SoundStream_Init(u32 threadPriority, void* heap) {
  int i;
  SoundStream* stream;

  if (gSoundStreamReady) {
    SoundStream_AllocBuffers(heap);
    return;
  }
  gSoundStreamReady = TRUE;
  LinkList_Init(&data_022a8ab8, 0);
  LinkList_Init(&data_022a8aac, 0);
  for (i = 0; i < SOUND_STREAM_REQUEST_COUNT; i++) {
    LinkList_Append(&data_022a8aac, &gSoundStreamRequests[i]);
  }
  data_022a8aa4 = data_022a8d00;
  stream = data_022a9300;
  for (i = 0; i < SOUND_STREAM_COUNT; i++, stream++) {
    stream->flags.fileOpen = 0;
    FS_InitFile(&stream->file);
    SoundStrm_Init(&stream->strm);
    stream->slotNo = i;
    stream->slotChannelCount = 0;
    stream->buffer = NULL;
    stream->bufferSize = 0;
    stream->channelLockCount = 0;
  }
  SoundStream_AllocBuffers(heap);
  func_0210e0c0(&data_022a8ad8);
  OS_CreateThread(&gSoundStreamThread, SoundStream_ThreadMain, NULL,
                  data_022a8d00 + SOUND_STREAM_READ_BUFFER_SIZE +
                      SOUND_STREAM_THREAD_STACK_SIZE,
                  SOUND_STREAM_THREAD_STACK_SIZE, threadPriority);
  OS_WakeupThreadDirect(&gSoundStreamThread);
}
