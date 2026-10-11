#include "types.h"
#include "game/sound_strm.h"

void SoundStrm_StopIfActive(SoundStrm *strm);
int SoundAlarm_Alloc(void);
void SoundStrm_Refill(SoundStrm *strm, int status);
void SoundStrm_OnAlarm(SoundStrm *strm);
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);
void func_0210fc04(int channel, int format, u32 buffer, int loop, int loopStart, u32 loopLength,
                   int volume, int shift, int timer, int pan);
void func_0210fd14(int alarm, u32 tick, u32 period, void (*handler)(SoundStrm *), SoundStrm *arg);

extern LinkList data_022a892c;             /* active streams */
extern SoundStrmChannel data_022a8978[SOUND_STRM_CHANNEL_COUNT];

/* NNS_SndStrmSetup: points each of the stream's hardware channels at its part
 * of buffer as a looping PCM ring, sets up an alarm that fires once per block
 * to refill it, and fills the whole ring once. timer is the channel timer
 * value; the alarm period works out to one block of samples. Returns FALSE
 * when no alarm is free. */
BOOL SoundStrm_Setup(SoundStrm *strm, int format, u32 buffer, u32 length, int timer, int blockCount,
                   SoundStrmCallback callback, void *callbackArg)
{
    u32 blockUnits;
    u32 period;
    int i;
    int irqState;

    if (strm->flags.active) {
        SoundStrm_StopIfActive(strm);
    }

    /* Each block a multiple of 32 bytes, the whole ring split between the channels. */
    blockUnits = length / (blockCount * 32 * strm->channelCount);
    strm->bufferSize = blockUnits * blockCount * 32;
    period = strm->bufferSize;
    if (format == STRM_FORMAT_PCM16) {
        period >>= 1;
    }
    period = timer * period / blockCount;

    strm->alarm = SoundAlarm_Alloc();
    if (strm->alarm < 0) {
        return FALSE;
    }

    for (i = 0; i < strm->channelCount; i++) {
        int channel = strm->channels[i];

        data_022a8978[channel].buffer = buffer + strm->bufferSize * i;
        data_022a8978[channel].volume = 0;
        func_0210fc04(channel, format, data_022a8978[channel].buffer, 1, 0, strm->bufferSize >> 2,
                      127, 0, timer * 32, 64);
    }
    func_0210fd14(strm->alarm, period, period, SoundStrm_OnAlarm, strm);
    LinkList_Append(&data_022a892c, strm);

    strm->format = format;
    strm->blockCount = blockCount;
    strm->callback = callback;
    strm->callbackArg = callbackArg;
    strm->blockIndex = 0;
    strm->volume = 0;
    strm->flags.active = 1;

    irqState = OS_DisableInterrupts();
    strm->blockCount = 1;
    SoundStrm_Refill(strm, STRM_CALLBACK_FIRST_FILL);
    strm->blockCount = blockCount;
    OS_RestoreInterrupts(irqState);
    return TRUE;
}
