#ifndef GAME_SOUND_STREAM_H
#define GAME_SOUND_STREAM_H

#include "types.h"
#include "game/sound_handle.h"
#include "game/sound_strm.h"
#include "sdk/fs.h"

/* Flag bits of SoundStream.flags. Signed one-bit fields: the original code
 * reads them with shift and arithmetic shift. */
typedef struct SoundStreamFlags {
    s32 fileOpen : 1;                  /* bit 0: the stream file is open */
    s32 playing : 1;                   /* bit 1: started on the hardware channels */
    s32 startPending : 1;              /* bit 2: start playing once the data is ready */
    s32 stopping : 1;                  /* bit 3: fading out, closes when the fade ends */
    s32 skipToPosition : 1;            /* bit 4: ADPCM start inside a block: decode up to samplePos first */
    s32 dataEnded : 1;                 /* bit 5: no more data to read: fill with silence */
    s32 silenceExtraChannels : 1;      /* bit 6: channels past the file's count get silence, not channel 0 */
} SoundStreamFlags;

/* SoundStream.format. An enum, not #defines: `format == PCM8 ? PCM8 : PCM16`
 * then compiles as the original does. */
enum {
    SOUND_STREAM_FORMAT_PCM8,
    SOUND_STREAM_FORMAT_PCM16,
    SOUND_STREAM_FORMAT_ADPCM
};

/* IMA ADPCM decoder state of one channel. Each ADPCM block starts with one. */
typedef struct AdpcmState {
    s16 sample;                        /* last decoded sample */
    u8 stepIndex;                      /* 0 to 88 */
    u8 pad_03;
} AdpcmState;

/* NitroSystem sound stream (NNSSndStrm), only the fields the game code
 * reads. */
/* What SoundStream.nextStreamCallback is told about the stream playing. */
typedef struct SoundNextStreamParam {
    u32 slotNo;
    int strmNo;
} SoundNextStreamParam;

/* What SoundStream.nextStreamCallback fills in: the stream to play next and
 * where to start in it, in milliseconds. */
typedef struct SoundNextStreamInfo {
    int strmNo;
    u32 offsetMs;
} SoundNextStreamInfo;

/* Returns FALSE when there is no next stream. status is 0 in the one call the
 * code makes. */
typedef BOOL (*SoundNextStreamCallback)(int status, const SoundNextStreamParam *param,
                                        SoundNextStreamInfo *info, void *arg);

typedef struct SoundStream {
    SoundStrm strm;                    /* 0x00 */
    FSFile file;                       /* 0x5c */
    u32 fileOffset;                    /* 0xa0: start of the stream file in the archive */
    u8 head[0x18];                     /* 0xa4: start of the stream file header; SOUND_STREAM_HEAD_SIZE bytes from here, up to 0xe4 */
    u8 format;                         /* 0xbc: SOUND_STREAM_FORMAT_* */
    u8 loop;                           /* 0xbd */
    u8 channelCount;                   /* 0xbe: channels in the file */
    u8 pad_bf;
    u16 sampleRate;                    /* 0xc0 */
    u16 timer;                         /* 0xc2: hardware channel timer for the sample rate */
    u32 loopStart;                     /* 0xc4: sample to go back to at the end */
    u8 pad_c8[4];
    u32 dataOffset;                    /* 0xcc: first data byte, from the file start */
    u32 blockCount;                    /* 0xd0 */
    u32 blockSize;                     /* 0xd4: bytes per block and channel */
    u32 blockSamples;                  /* 0xd8: samples per block */
    u32 lastBlockSize;                 /* 0xdc */
    u32 lastBlockSamples;              /* 0xe0 */
    SoundFader volumeFader;            /* 0xe4 */
    AdpcmState adpcm[2];               /* 0xf4: decoder state per channel */
    u8 pad_fc[0x10];
    SoundStreamFlags flags;            /* 0x10c */
    int unk_110;                       /* 0: close at once */
    int unk_114;                       /* nonzero when the data is ready */
    int pendingRequests;               /* 0x118: refill requests queued for the sound thread */
    int channelLockCount;              /* 0x11c: users of the hardware channels; locked on the first, unlocked on the last */
    u8 slotChannelCount;               /* 0x120: hardware channels the stream's slot may use */
    u8 pad_121;
    u8 slotChannels[6];                /* 0x122: their numbers */
    void *buffer;                      /* 0x128: block memory taken from the sound heap; NULL: slot unused */
    u32 bufferSize;                    /* 0x12c */
    SoundStrmCallback callback;        /* 0x130: called after each refill */
    void *callbackArg;                 /* 0x134 */
    SoundNextStreamCallback nextStreamCallback; /* 0x138: asked for the stream to play once the data ends */
    void *nextStreamArg;               /* 0x13c: passed to nextStreamCallback */
    int strmNo;                        /* 0x140: stream number in the sound archive */
    u32 slotNo;                        /* 0x144: index in data_022a9300 */
    void **handle;                     /* 0x148: the caller's stream handle, which points back here */
    int priority;                      /* 0x14c: of the handle that holds the slot */
    int volumeIndex;                   /* 0x150: index into the volume table */
    int appliedVolume;                 /* 0x154: last volume sent to the hardware */
    u32 samplePos;                     /* 0x158: next sample to read */
} SoundStream;

/* A refill of a stream's blocks, queued by the stream driver's callback and
 * handled by the sound thread. */
typedef struct SoundStreamRequest {
    ListLink link;                     /* 0x00 */
    SoundStream *stream;               /* 0x08 */
    int status;                        /* 0x0c: STRM_CALLBACK_* */
    int channelCount;                  /* 0x10 */
    void *blocks[6];                   /* 0x14: one per channel */
    u32 blockSize;                     /* 0x2c: bytes per block */
} SoundStreamRequest;

#define SOUND_STREAM_COUNT 4

/* Bytes of the stream file header copied to SoundStream.head. */
#define SOUND_STREAM_HEAD_SIZE 0x40

extern SoundStream data_022a9300[SOUND_STREAM_COUNT];

#endif
