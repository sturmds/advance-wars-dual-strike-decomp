#ifndef GAME_SOUND_ARC_H
#define GAME_SOUND_ARC_H

#include "types.h"

/* NitroSystem sound archive (SDAT) structures, only the fields the code
 * in src/ reads. Offsets inside the info block are relative to the start
 * of that block; offset 0 means the item is absent. */

/* A sequence entry: the file plus its playback parameters. */
typedef struct SoundArcSeqInfo {
    u32 fileId;
    u16 bankNo;
    u8 volume;
    u8 channelPrio;
    u8 playerPrio;
    u8 playerNo;
    u16 pad_0a;
} SoundArcSeqInfo;

/* Table of sequence entries, each given as an offset into the info block. */
typedef struct SoundArcSeqInfoTable {
    u32 count;
    u32 offset[1];                     /* count entries */
} SoundArcSeqInfoTable;

/* Start of the archive's INFO block. */
typedef struct SoundArcInfo {
    char signature[4];
    u32 blockSize;
    u32 seqOffset;                     /* SoundArcSeqInfoTable */
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 strmOffset;                    /* table of stream entries */
} SoundArcInfo;

/* NitroSystem NNSSndArc: an opened sound archive. */
typedef struct SoundArc {
    u8 pad_00[0x88];
    SoundArcInfo *info;
} SoundArc;

#endif
