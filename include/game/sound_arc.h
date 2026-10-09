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

/* A stream entry: the file plus its playback parameters. */
typedef struct SoundArcStrmInfo {
    u32 fileId;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
} SoundArcStrmInfo;

/* A bank entry: the file plus the wave archives it uses (0xffff: slot unused). */
typedef struct SoundArcBankInfo {
    u32 fileId;
    u16 waveArc[4];
} SoundArcBankInfo;

#define SOUND_ARC_NO_WAVE_ARC 0xffff

/* A wave archive entry: the file id in the low 24 bits and a flag byte on
 * top. Needed to match: the flag byte is read on its own. */
typedef struct SoundArcWaveArcFileId {
    u32 fileId : 24;
} SoundArcWaveArcFileId;

typedef union SoundArcWaveArcInfo {
    SoundArcWaveArcFileId id;
    struct {
        u8 pad_00[3];
        u8 flags;                      /* bit 0: wave data is loaded on demand */
    } b;
} SoundArcWaveArcInfo;

/* A sequence archive entry. */
typedef struct SoundArcSeqArcInfo {
    u32 fileId;
} SoundArcSeqArcInfo;

/* A sequence archive file or a sequence file as loaded in memory. */
typedef struct SoundSeqArcFile {
    u8 pad_00[0x18];
    u32 dataOffset;                    /* start of the sequence data, from the start of the file */
} SoundSeqArcFile;

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
    u32 unk_24;
} SoundArcInfo;

/* NitroSystem NNSSndArc: an opened sound archive. */
typedef struct SoundArc {
    u8 pad_00[0x88];
    SoundArcInfo *info;
} SoundArc;

/* Load an item of the current archive into a sound heap; TRUE on success. */
BOOL NNS_SndArcLoadSeqArc(int seqArcNo, void *heap);
BOOL NNS_SndArcLoadBank(int bankNo, void *heap);

#endif
