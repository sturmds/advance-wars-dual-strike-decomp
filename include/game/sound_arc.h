#ifndef GAME_SOUND_ARC_H
#define GAME_SOUND_ARC_H

#include "types.h"
#include "sdk/fs.h"

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
    u8 volume;                         /* 0x04: index into the stream volume table */
    u8 priority;
    u8 slotNo;                         /* 0x06: stream slot */
    u8 flags;                          /* 0x07: bit 0 forces two channels */
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
    u32 seqCount;                      /* 0x1c */
    SoundArcSeqInfo seqs[1];           /* 0x20: seqCount entries; fileId 0xffffffff: unused */
} SoundSeqArcFile;

/* Table of sequence entries, each given as an offset into the info block. */
typedef struct SoundArcSeqInfoTable {
    u32 count;
    u32 offset[1];                     /* count entries */
} SoundArcSeqInfoTable;

/* A player group entry (table at SoundArcInfo.unk_18): how many sequence
 * players the group runs and the heap size reserved for each. */
typedef struct SoundArcPlayerGroupInfo {
    u8 maxPlayers;
    u8 pad_01;
    u16 trackChannelMask;
    u32 heapSize;
} SoundArcPlayerGroupInfo;

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

/* Two words of an archive, a start and an end by their use. */
typedef struct SoundArcRange {
    u32 unk_00;
    u32 unk_04;
} SoundArcRange;

/* One file of the archive's file table: where it sits in the archive file and
 * where it is loaded, if it is. */
typedef struct SoundArcFileEntry {
    u32 offset;                        /* from the start of the archive file */
    u32 size;
    void *loaded;                      /* memory copy, NULL when not loaded */
    u32 unk_0c;
} SoundArcFileEntry;

typedef struct SoundArcFileTable {
    u32 unk_00;
    u32 unk_04;
    u32 count;
    SoundArcFileEntry entries[1];      /* count entries */
} SoundArcFileTable;

/* Bytes of the archive file header, copied to the start of SoundArc. */
#define SOUND_ARC_HEADER_SIZE 0x30

/* NitroSystem NNSSndArc: an opened sound archive. */
typedef struct SoundArc {
    u8 pad_00[0x10];                   /* 0x00-0x2f: copy of the archive file's header */
    u32 symbolOffset;                  /* 0x10: symbol block, from the start of the file; read when symbolSize is not 0 */
    u32 symbolSize;
    u32 infoOffset;                    /* 0x18: INFO block, read into info */
    u32 infoSize;
    u32 fatOffset;                     /* 0x20: file table, read into files */
    u32 fatSize;
    u8 pad_28[0x30 - 0x28];
    int opened;                       /* 0x30: set once the archive file is open */
    FSFile file;                      /* 0x34: the archive file */
    SoundArcRange range;               /* 0x78: handed to the ROM direct file open call */
    SoundArcFileTable *files;          /* 0x80 */
    void *symbols;                     /* 0x84: symbol block in a sound heap; NULL when not read */
    SoundArcInfo *info;
} SoundArc;

/* Load an item of the current archive into a sound heap; TRUE on success. */
BOOL NNS_SndArcLoadSeqArc(int seqArcNo, void *heap);
BOOL NNS_SndArcLoadBank(int bankNo, void *heap);

#endif
