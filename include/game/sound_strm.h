#ifndef GAME_SOUND_STRM_H
#define GAME_SOUND_STRM_H

#include "types.h"
#include "game/link_list.h"

/* Callback of the system's sleep and wake-up hooks (a 12-byte node in a
 * singly linked list). */
typedef struct SoundSleepHook {
  void (*callback)(void* arg); /* 0x00 */
  void* arg;                   /* 0x04 */
  struct SoundSleepHook* next; /* 0x08 */
} SoundSleepHook;

/* SoundStrm.format */
#define STRM_FORMAT_PCM8 0
#define STRM_FORMAT_PCM16 1

/* First argument of the SoundStrm callback. */
#define STRM_CALLBACK_FIRST_FILL 0 /* fill before playback starts */
#define STRM_CALLBACK_REFILL 1     /* a block has played, fill it again */

/* Signed one-bit fields, read with shift and arithmetic shift. */
typedef struct SoundStrmFlags {
  s32 active : 1;  /* bit 0: in the stream list, owns an alarm and channels */
  s32 running : 1; /* bit 1: channels started, sleep hooks registered */
} SoundStrmFlags;

typedef void (*SoundStrmCallback)(int status, int channelCount, void* blocks[],
                                  u32 blockSize, int format, void* arg);

/* Where the hardware channels of a stream read from. One per hardware
 * channel, array at data_022a8978. */
typedef struct SoundStrmChannel {
  u32 buffer; /* address of the ring */
  int volume; /* added to the stream volume for this channel */
} SoundStrmChannel;

#define SOUND_STRM_CHANNEL_COUNT 16

/* NitroSystem stream driver (NNSSndStrm): plays a ring of blocks on one or
 * more hardware channels and calls a callback to refill each block as it has
 * played. 0x5c bytes, at the start of SoundStream. */
typedef struct SoundStrm {
  ListLink link;              /* 0x00: link in the list of active streams */
  SoundSleepHook preSleep;    /* 0x08: stops the channels */
  SoundSleepHook postSleep;   /* 0x14: refills and restarts them */
  int format;                 /* 0x20: STRM_FORMAT_* */
  SoundStrmFlags flags;       /* 0x24 */
  u32 bufferSize;             /* 0x28: bytes in the ring, per channel */
  int blockCount;             /* 0x2c: blocks in the ring */
  SoundStrmCallback callback; /* 0x30 */
  void* callbackArg;          /* 0x34 */
  int blockIndex;             /* 0x38: next block to fill */
  int volume;                 /* 0x3c */
  int alarm;        /* 0x40: sound alarm number that triggers refills */
  u32 channelMask;  /* 0x44: bit per hardware channel in use, 0 for none */
  int channelCount; /* 0x48 */
  u8 channels[SOUND_STRM_CHANNEL_COUNT]; /* 0x4c: hardware channel numbers */
} SoundStrm;

#endif
