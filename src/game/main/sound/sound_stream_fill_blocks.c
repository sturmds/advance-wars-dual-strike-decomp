#include "types.h"
#include "game/sound_stream.h"
#include "sdk/os.h"

extern s16 data_0214d410[]; /* ADPCM step sizes, 89 entries */
extern s8 data_0214d400[];  /* ADPCM step index change per nibble */

void MI_CpuFill8(void* dest, int value, u32 size);
void Mem_CopyBytes(const void* src, void* dest, u32 size);
void SoundStream_SetupNext(SoundStream* stream);

/* IMA ADPCM: decode one 4-bit code with a channel's state and store the
 * 16-bit sample in dst. A macro with its own locals, not an inline function:
 * the inline version gets other registers than the original. */
#define ADPCM_DECODE(state, nib, dst) \
  do {                                \
    u32 nibble;                       \
    s32 step;                         \
    s32 sample;                       \
    s32 index;                        \
    s32 delta;                        \
    nibble = (nib);                   \
    sample = (state)->sample;         \
    index = (state)->stepIndex;       \
    step = data_0214d410[index];      \
    delta = step >> 3;                \
    if (nibble & 4)                   \
      delta += step;                  \
    if (nibble & 2)                   \
      delta += step >> 1;             \
    if (nibble & 1)                   \
      delta += step >> 2;             \
    if (nibble & 8) {                 \
      sample -= delta;                \
      if (sample < -0x8000)           \
        sample = -0x8000;             \
    } else {                          \
      sample += delta;                \
      if (sample > 0x7fff)            \
        sample = 0x7fff;              \
    }                                 \
    index += data_0214d400[nibble];   \
    if (index < 0)                    \
      index = 0;                      \
    else if (index > 88)              \
      index = 88;                     \
    (state)->sample = sample;         \
    (state)->stepIndex = index;       \
    dst = (s16)sample;                \
  } while (0)

/* Refill the blocks of one stream request from the stream file: read PCM8 or
 * PCM16 data straight into the blocks, or decode ADPCM into them, then call
 * the stream's callback. */
void SoundStream_FillBlocks(SoundStreamRequest* request) {
  SoundStream* stream = request->stream;
  BOOL loopBack;
  u32 offset;
  u32 remaining;
  u32 blockSize;
  u32 blockPos;
  u32 filePos;
  u32 samples;
  u32 bytes;
  u32 readSize;
  u32 end;
  int ch;
  u32 block;
  u32 blockSamples;
  u32 readOffset;
  u8* dest;
  s16* out;
  u8* src;
  AdpcmState* state;
  u32 i;
  u32 evenEnd;
  s32 bytesRead;

  if (stream->flags.dataEnded) {
    /* volatile: the original reads unk_110 again after the test */
    if (*(volatile int*)&stream->unk_110 > 0) {
      stream->unk_110--;
    }
  }

  remaining = request->blockSize;
  offset = 0;
  while (remaining != 0) {
    if (stream->flags.dataEnded) {
      for (ch = 0; ch < request->channelCount; ch++) {
        MI_CpuFill8((u8*)request->blocks[ch] + offset, 0, remaining);
      }
      break;
    }

    block = stream->samplePos / stream->blockSamples;
    if (block < stream->blockCount - 1) {
      blockSize = stream->blockSize;
      blockSamples = stream->blockSamples;
    } else {
      blockSize = stream->lastBlockSize;
      blockSamples = stream->lastBlockSamples;
    }
    blockPos = stream->samplePos - block * stream->blockSamples;

    samples = remaining;
    if (stream->format != SOUND_STREAM_FORMAT_PCM8) {
      samples = remaining >> 1;
    }
    if (stream->flags.skipToPosition) {
      if (blockPos == 0) {
        stream->flags.skipToPosition = 0;
      } else {
        samples = blockPos;
        blockPos = 0;
      }
    }

    loopBack = FALSE;
    if (blockPos + samples >= blockSamples) {
      samples = blockSamples - blockPos;
      if (block >= stream->blockCount - 1) {
        if (stream->loop) {
          loopBack = TRUE;
        } else {
          stream->flags.dataEnded = 1;
        }
      }
    }

    readOffset = blockPos;
    bytes = samples;
    switch (stream->format) {
      case SOUND_STREAM_FORMAT_PCM8:
        readSize = samples;
        break;
      case SOUND_STREAM_FORMAT_PCM16:
        readOffset <<= 1;
        readSize = bytes = samples << 1;
        break;
      case SOUND_STREAM_FORMAT_ADPCM:
        readOffset >>= 1;
        readSize = ((blockPos + samples + 1) >> 1) - readOffset;
        if (blockPos == 0) {
          readSize += 4;
        } else {
          readOffset += 4;
        }
        bytes <<= 1;
        break;
    }

    /* Needed to match: the sum is built in three statements. */
    filePos = block * stream->blockSize * stream->channelCount + readOffset;
    filePos += stream->dataOffset;
    filePos += stream->fileOffset;

    for (ch = 0; ch < request->channelCount; ch++) {
      state = &stream->adpcm[ch];
      dest = (u8*)request->blocks[ch] + offset;
      out = (s16*)dest;
      if (ch < stream->channelCount) {
        if (stream->format == SOUND_STREAM_FORMAT_ADPCM) {
          dest = data_022a8aa4;
        }
        FS_SeekFile(&stream->file, filePos + ch * blockSize, 0);
        bytesRead = FS_ReadFile(&stream->file, dest, readSize);
        if (bytesRead != readSize) {
          /* read error: end the stream */
          loopBack = samples = bytes = 0;
          stream->flags.dataEnded = 1;
          break;
        }
        if (stream->format == SOUND_STREAM_FORMAT_ADPCM) {
          src = data_022a8aa4;
          if (blockPos == 0) {
            AdpcmState* header = (AdpcmState*)src;
            src += 4;
            *state = *header;
          }
          i = blockPos;
          end = blockPos + samples;
          if (blockPos & 1) {
            ADPCM_DECODE(state, (*src >> 4) & 0xf, *out++);
            i++;
            src++;
          }
          evenEnd = end & ~1;
          for (; i < evenEnd; i += 2, src++) {
            ADPCM_DECODE(state, *src & 0xf, *out++);
            ADPCM_DECODE(state, (*src >> 4) & 0xf, *out++);
          }
          if (i < end) {
            ADPCM_DECODE(state, *src & 0xf, *out);
          }
        }
      } else if (stream->flags.silenceExtraChannels) {
        MI_CpuFill8(dest, 0, bytes);
      } else {
        Mem_CopyBytes((u8*)request->blocks[0] + offset, dest, bytes);
      }
    }

    if (stream->flags.skipToPosition) {
      stream->flags.skipToPosition = 0;
    } else {
      if (loopBack) {
        stream->samplePos = stream->loopStart;
      } else {
        stream->samplePos += samples;
      }
      offset += bytes;
      remaining -= bytes;
      if (stream->flags.dataEnded && stream->nextStreamCallback != NULL) {
        SoundStream_SetupNext(stream);
      }
    }
  }

  if (stream->callback != NULL) {
    stream->callback(request->status, request->channelCount, request->blocks,
                     request->blockSize,
                     /* ADPCM is decoded to PCM16 */
                     stream->format == SOUND_STREAM_FORMAT_PCM8
                         ? SOUND_STREAM_FORMAT_PCM8
                         : SOUND_STREAM_FORMAT_PCM16,
                     stream->callbackArg);
  }
  for (ch = 0; ch < request->channelCount; ch++) {
    DC_FlushRange(request->blocks[ch], request->blockSize);
  }
  if (request->status == STRM_CALLBACK_FIRST_FILL) {
    stream->unk_114 = 1;
  }
}
