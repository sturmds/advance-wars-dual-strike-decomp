#include "types.h"
#include "game/sound_arc.h"
#include "game/sound_stream.h"

const SoundArcStrmInfo *NNS_SndArcGetStrmInfo(int strmNo);
u32 func_021063f8(u32 fileNo);
s32 SoundArc_ReadFile(u32 fileNo, void *dest, u32 size, u32 offset);

/* Asks the stream's next-stream callback what to play once the data ends.
 * When the archive has that stream, reads its header over the current one;
 * when the sample rate matches and both streams are PCM8 or both are not,
 * switches to it, starting offsetMs milliseconds in. */
void SoundStream_SetupNext(SoundStream *stream)
{
    SoundNextStreamParam param;
    SoundNextStreamInfo info;
    const SoundArcStrmInfo *strmInfo;
    u8 oldFormat;
    u16 oldSampleRate;

    param.slotNo = stream->slotNo;
    param.strmNo = stream->strmNo;
    info.strmNo = stream->strmNo;
    info.offsetMs = 0;
    if (!((SoundNextStreamCallback)stream->nextStreamCallback)(0, &param, &info, stream->nextStreamArg)) {
        return;
    }
    strmInfo = NNS_SndArcGetStrmInfo(info.strmNo);
    if (strmInfo == NULL) {
        return;
    }

    oldFormat = stream->format;
    oldSampleRate = stream->sampleRate;
    stream->fileOffset = func_021063f8(strmInfo->fileId);
    SoundArc_ReadFile(strmInfo->fileId, stream->head, SOUND_STREAM_HEAD_SIZE, 0);
    if (oldSampleRate != stream->sampleRate) {
        return;
    }
    if (oldFormat == SOUND_STREAM_FORMAT_PCM8 && stream->format != SOUND_STREAM_FORMAT_PCM8) {
        return;
    }
    if (oldFormat != SOUND_STREAM_FORMAT_PCM8 && stream->format == SOUND_STREAM_FORMAT_PCM8) {
        return;
    }

    stream->strmNo = info.strmNo;
    stream->samplePos = stream->sampleRate * info.offsetMs / 1000;
    if (stream->samplePos != 0 && stream->format == SOUND_STREAM_FORMAT_ADPCM) {
        stream->flags.skipToPosition = 1;
    } else {
        stream->flags.skipToPosition = 0;
    }
    stream->flags.dataEnded = 0;
}
