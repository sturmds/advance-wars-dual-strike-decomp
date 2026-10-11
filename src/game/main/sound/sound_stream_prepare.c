#include "types.h"
#include "game/sound_arc.h"
#include "game/sound_stream.h"

SoundStream *SoundStream_Attach(void **handle, int slotNo, int priority);
void SoundStream_Detach(SoundStream *stream);
BOOL SoundStream_LockChannels(SoundStream *stream, int count, const u8 *channels);
void SoundStream_UnlockChannels(SoundStream *stream);
s32 SoundArc_ReadFile(u32 fileNo, void *dest, u32 size, u32 offset);
void SoundArc_GetRange(SoundArcRange *out);
BOOL FS_OpenFileFast(FSFile *file, SoundArcRange range);
u32 SoundArc_GetFileOffset(u32 fileNo);
void func_02109328(SoundFader *fader);
void SoundFader_Start(SoundFader *fader, int target, int frames);
BOOL SoundStrm_Setup(SoundStrm *strm, int format, void *buffer, u32 length, int timer, int blockCount,
                     SoundStrmCallback callback, void *callbackArg);
void SoundStrm_SetPan(SoundStrm *strm, int index, int pan);
void SoundStream_QueueRefill(int status, int channelCount, void *blocks[], u32 blockSize, int format,
                             SoundStream *stream);

/* Takes a stream slot of group slotNo for handle and opens stream file
 * info->fileId on it: reads its header, sets the start offsetMs milliseconds
 * in, the callbacks, and the volume from info. Then locks hardware channels
 * (two when info->flags bit 0 is set, else the file's count, at most the
 * slot's) and sets up the stream driver with four blocks; two channels play
 * hard left and hard right. Returns FALSE and gives the slot back when a step
 * fails. */
BOOL SoundStream_Prepare(void **handle, const SoundArcStrmInfo *info, int slotNo, int priority, int strmNo,
                   u32 offsetMs, SoundStrmCallback callback, void *callbackArg, SoundNextStreamCallback nextStreamCallback,
                   void *nextStreamArg)
{
    SoundStream *stream;
    SoundArcRange range;
    int strmFormat;
    int channelCount;

    stream = SoundStream_Attach(handle, slotNo, priority);
    if (stream == NULL) {
        return FALSE;
    }
    if (SoundArc_ReadFile(info->fileId, stream->head, SOUND_STREAM_HEAD_SIZE, 0) != SOUND_STREAM_HEAD_SIZE) {
        SoundStream_Detach(stream);
        return FALSE;
    }
    SoundArc_GetRange(&range);
    if (!FS_OpenFileFast(&stream->file, range)) {
        SoundStream_Detach(stream);
        return FALSE;
    }

    stream->fileOffset = SoundArc_GetFileOffset(info->fileId);
    stream->samplePos = stream->sampleRate * offsetMs / 1000;
    if (stream->samplePos != 0 && stream->format == SOUND_STREAM_FORMAT_ADPCM) {
        stream->flags.skipToPosition = 1;
    } else {
        stream->flags.skipToPosition = 0;
    }
    stream->unk_110 = 4;
    stream->flags.dataEnded = 0;
    stream->flags.playing = 0;
    stream->unk_114 = 0;
    stream->flags.startPending = 0;
    stream->flags.stopping = 0;
    stream->pendingRequests = 0;
    stream->callback = callback;
    stream->callbackArg = callbackArg;
    stream->nextStreamCallback = nextStreamCallback;
    stream->nextStreamArg = nextStreamArg;
    stream->strmNo = strmNo;
    stream->appliedVolume = 0;
    stream->volumeIndex = info->volume;
    func_02109328(&stream->volumeFader);
    SoundFader_Start(&stream->volumeFader, 127 << 8, 1);

    switch (stream->format) {
    case SOUND_STREAM_FORMAT_PCM8:
        strmFormat = STRM_FORMAT_PCM8;
        break;
    case SOUND_STREAM_FORMAT_PCM16:
    case SOUND_STREAM_FORMAT_ADPCM:
        strmFormat = STRM_FORMAT_PCM16;
        break;
    }

    channelCount = stream->channelCount;
    if (info->flags & 1) {
        channelCount = 2;
    }
    if (channelCount > stream->slotChannelCount) {
        channelCount = stream->slotChannelCount;
    }
    stream->flags.silenceExtraChannels = (channelCount == 1);
    if (!SoundStream_LockChannels(stream, channelCount, stream->slotChannels)) {
        FS_CloseFile(&stream->file);
        SoundStream_Detach(stream);
        return FALSE;
    }
    if (!SoundStrm_Setup(&stream->strm, strmFormat, stream->buffer,
                         stream->bufferSize * channelCount / stream->slotChannelCount, stream->timer, 4,
                         (SoundStrmCallback)SoundStream_QueueRefill, stream)) {
        SoundStream_UnlockChannels(stream);
        FS_CloseFile(&stream->file);
        SoundStream_Detach(stream);
        return FALSE;
    }
    if (channelCount == 2) {
        SoundStrm_SetPan(&stream->strm, 0, 0);
        SoundStrm_SetPan(&stream->strm, 1, 127);
    }
    return TRUE;
}
