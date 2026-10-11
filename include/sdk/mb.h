#ifndef SDK_MB_H
#define SDK_MB_H

#include "types.h"

/* Download Play (MB) parent side: the games the parent offers, and the
 * beacon that advertises them. */

#define DLPLAY_ICON_COLOR_BYTES 0x20
#define DLPLAY_ICON_PIXEL_BYTES 0x200
#define DLPLAY_GAME_EXTRA_BYTES 22
#define DLPLAY_TITLE_CHARS 48
#define DLPLAY_BLURB_CHARS 96
#define DLPLAY_CHILD_SLOTS 15
#define DLPLAY_PLAYER_BADGE_BYTES 22

/* One game on the parent's list. The beacon sends it in 98-byte pieces,
 * from iconPalette (game has an icon) or from userParam (no icon). */
typedef struct DlPlayGameEntry {
    u8 iconPalette[DLPLAY_ICON_COLOR_BYTES];
    u8 iconChars[DLPLAY_ICON_PIXEL_BYTES];
    u8 userParam[DLPLAY_GAME_EXTRA_BYTES];
    u8 maxPlayers;
    u8 pad_237;
    u16 gameName[DLPLAY_TITLE_CHARS];
    u16 gameIntro[DLPLAY_BLURB_CHARS];
    u8 playerCount;                    /* parent plus joined children */
    u8 pad_359;
    u16 playerFlags;                   /* bit 0 is the parent */
    u16 unk_35c;
    u8 users[DLPLAY_CHILD_SLOTS][DLPLAY_PLAYER_BADGE_BYTES];
    u8 unk_4a8[8];                     /* sent at the end of the player beacon */
    u16 changed;                       /* set to 1 when the beacon sent the game's pieces */
    u8 noIcon;                         /* 1 when an icon file could not be read */
    u8 unk_4b3;
    u8 changeCount;                    /* up by one at each player change */
    u8 unk_4b5;
    u8 pad_4b6[2];
    u32 ggid;
    struct DlPlayGameEntry *next;           /* list link */
} DlPlayGameEntry;

/* What the parent is told about a game when it registers it. */
typedef struct DlPlayGameSource {
    const char *romFilePath;
    const u16 *gameName;
    const u16 *gameIntro;
    const char *iconCharPath;
    const char *iconPalettePath;
    u32 ggid;
    u8 maxPlayers;
} DlPlayGameSource;

/* Beacon state values. */
#define DLPLAY_ADVERT_STEP_OFF 0
#define DLPLAY_ADVERT_STEP_PICK 1            /* pick the next game */
#define DLPLAY_ADVERT_STEP_SPLIT 2             /* split the game into pieces */
#define DLPLAY_ADVERT_STEP_EMIT 3             /* send one piece per call */
#define DLPLAY_ADVERT_STEP_DONE 4
#define DLPLAY_ADVERT_STEP_ROSTER 5          /* send the player list */

/* data_022ace4c */
typedef struct DlPlayAdvertProgress {
    DlPlayGameEntry *gameList;
    DlPlayGameEntry *currentGame;
    u8 *nextPiece;                     /* next byte of the game to send */
    u8 state;
    u8 pad_0d;
    u8 changeCount;                    /* copy of the game's changeCount */
    u8 pieceIndex;
    u8 pieceCount;
    u8 sequence;
} DlPlayAdvertProgress;

#define DLPLAY_ADVERT_CHUNK_BYTES 98
#define DLPLAY_ADVERT_SUMMED_BYTES 0x68       /* from the checksum field to the end of DlPlayAdvertFrame */

/* The user game info the parent broadcasts, 0x70 bytes at data_022ace60. */
typedef struct DlPlayAdvertFrame {
    u32 ggid;
    u8 noIcon : 2;
    u8 unk_4b5 : 6;
    u8 unk_4b3;
    u8 changeCount;
    u8 sequence;                       /* up by one at each beacon */
    u16 checksum;                      /* over the bytes from here to the end */
    u8 pieceIndex;
    u8 pieceCount;
    u8 size;                           /* bytes used in data */
    u8 pad_0d;
    u8 data[DLPLAY_ADVERT_CHUNK_BYTES];
} DlPlayAdvertFrame;

#define DLPLAY_ADVERT_IS_ROSTER 2           /* value of noIcon/kind in a player beacon */
#define DLPLAY_ROSTER_PER_FRAME 4

/* The same 0x70 bytes as DlPlayAdvertFrame, as sent in state DLPLAY_ADVERT_STEP_ROSTER:
 * up to four players' user info per beacon. */
typedef struct DlPlayRosterFrame {
    u32 ggid;
    u8 kind : 2;                       /* DLPLAY_ADVERT_IS_ROSTER */
    u8 unk_4b5 : 6;
    u8 unk_4b3;
    u8 changeCount;
    u8 sequence;
    u16 checksum;
    u8 playerCount;
    u8 pad_0b;
    u16 playerFlags;
    u16 unk_35c;
    u8 users[DLPLAY_ROSTER_PER_FRAME][DLPLAY_PLAYER_BADGE_BYTES];   /* a player number of 0 ends the list */
    u8 unk_4a8[8];
} DlPlayRosterFrame;

/* The parent's work area, behind data_022aced8. Only the fields the beacon
 * and send code read are named. */
typedef struct DlPlayStationArea {
    u8 pad_00[0x0c];
    u16 unk_0c;
    u16 unk_0e;
    u16 maxChildren;                   /* set by DownloadPlay_SetSendSizeAndChildren */
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    u8 pad_18[0x320 - 0x18];
    u32 unk_320;                       /* 0 until the game starts Download Play */
    u8 pad_324[0x500 - 0x324];
    u16 sendSize;                      /* set by DownloadPlay_SetSendSizeAndChildren */
    u16 unk_502;
    u8 pad_504[4];
    void (*sendCallback)(void);        /* callback for the MP send */
    u8 sendQueued;                     /* set to 1 once MbParent_Send queued the data */
    u8 unk_50d;
    u8 pad_50e[0x51c - 0x50e];
    void (*errorCallback)(u32 type, void *arg);
    u8 pad_520[4];
    u16 unk_524;
    u16 unk_526;
    u16 unk_528;
    u16 unk_52a;
    u16 unk_52c;
    u8 pad_52e[0x548 - 0x52e];
    u16 unk_548;
    u8 pad_54a[0x5e4 - 0x54a];
    u32 unk_5e4;
} DlPlayStationArea;

/* data_022aced4 points to the MB buffer; the parent work area sits 0x1000
 * bytes into it, which is what data_022aced8 points to. */
typedef struct DlPlayBufferArea {
    u8 pad_000[0x1000];
    DlPlayStationArea parent;
} DlPlayBufferArea;

/* Event types of the parent's callbacks (errorCallback, MbParent_CommCallback). */
#define DLPLAY_EVENT_CHILD_JOINED 0
#define DLPLAY_EVENT_CHILD_LEFT 1
#define DLPLAY_EVENT_HOST_FRAME_IN 3
#define DLPLAY_EVENT_SHUTDOWN_DONE 17
#define DLPLAY_EVENT_STARTUP_DONE 21
#define DLPLAY_EVENT_MAY_TRANSMIT 25
#define DLPLAY_EVENT_BEACON_OUT 28
#define DLPLAY_EVENT_RADIO_FAULT_OLD 0x100         /* a WM call failed; the argument is its WMCallback */

/* What errorCallback receives for type DLPLAY_EVENT_RADIO_FAULT. */
#define DLPLAY_EVENT_RADIO_FAULT 0xff
#define DLPLAY_EVENT_PORT_DATA 9            /* data arrived on the MB port */

typedef struct DlPlayFailedCall {
    u16 apiid;
    u16 errcode;
} DlPlayFailedCall;

/* One block of a child program kept in memory, 16 bytes. */
typedef struct DlPlayKeptChunk {
    u32 romAddress;                    /* where the block starts in the ROM */
    u32 length;
    u8 *buffer;
    u32 state;                         /* DLPLAY_KEPT_CHUNK_*, 0 when the slot is free */
} DlPlayKeptChunk;

#define DLPLAY_KEPT_CHUNK_USABLE 2
#define DLPLAY_KEPT_CHUNK_PINNED 3            /* kept for the whole download */

/* The blocks of one child program held in memory, 0x70 bytes. */
typedef struct DlPlayKeptChunkSet {
    u8 pad_00[0x10];
    char archiveName[4];               /* archive the program is read from */
    u32 archiveNameLength;
    u8 pad_18[0x18];
    DlPlayKeptChunk list[4];               /* 0x30: header, ARM9 static, ARM7 static (or ARM9 parts) */
} DlPlayKeptChunkSet;

/* Where a child program's ARM9 and ARM7 static code lie in the ROM, 16 bytes. */
typedef struct DlPlayProgramRomOffsets {
    u32 unk_00;
    u32 arm9RomAddress;
    u32 arm7RomAddress;
    u32 unk_0c;
} DlPlayProgramRomOffsets;

/* How DownloadPlay_LoadChildProgram copies part of the running program's ARM9 code into
 * the buffer it sends: ROM offset x comes from source + x and goes to
 * dest + x, for x in [romOffset, romOffset + length). */
typedef struct DlPlayLiveCodePatch {
    u32 romOffset;
    const u8 *source;
    u8 *dest;
    u32 length;
} DlPlayLiveCodePatch;

/* The cartridge's secure area, ROM offsets 0x4000 to 0x8000. */
#define DLPLAY_PROTECTED_ROM_START 0x4000
#define DLPLAY_PROTECTED_ROM_END 0x8000

/* Sizes DownloadPlay_LoadChildProgram uses when it splits the buffer. */
#define DLPLAY_SIGNATURE_BYTES 0x88
#define DLPLAY_ASSUMED_CART_BYTES 0x1000000      /* when the header gives no ROM size */
#define DLPLAY_MAINCPU_SPLIT_BYTES 0x4400           /* ARM9 code goes in three parts when both programs do not fit */
#define DLPLAY_RETURN_OPCODE 0xe12fff1e            /* ARM "bx lr" */
#define DLPLAY_PREFIX_BYTES 0x268     /* header, auth code, cache list and card map */
#define DLPLAY_PROGRAM_BUFFER_FLOOR 0x10000

#define DLPLAY_OFFER_SLOTS_OLD 16

/* One file (a child program) the parent offers, 0x5d4 bytes. */
typedef struct DlPlayOfferedFileOld {
    u8 downloadInfo[0xe4];             /* entry points, segments and auth code sent to children */
    DlPlayGameEntry gameInfo;               /* 0xe4 */
    u8 blockTable[0x14];               /* 0x5a4 */
    const DlPlayGameSource *registry;    /* 0x5b8 */
    const void *source;                /* 0x5bc: ROM header, auth code, cache list, card map */
    u8 pad_5c0[6];
    u16 childFlags;                    /* 0x5c6 */
    u8 pad_5c8[2];
    u8 active;                         /* 0x5ca */
    u8 pad_5cb;
    DlPlayKeptChunkSet *cacheList;            /* 0x5cc */
    DlPlayProgramRomOffsets *cardMapping;        /* 0x5d0 */
} DlPlayOfferedFileOld;

/* Offsets into the buffer given with a file. */
#define DLPLAY_SOURCE_KEPT_LIST_AT 0x1e8
#define DLPLAY_SOURCE_ROM_OFFSETS_AT 0x258

#define DLPLAY_WORKER_STACK_BYTES 0x800

/* The parent's file work area, behind data_022aced4. */
typedef struct DlPlayHostLinkAreaOld {
    u8 pad_0000[0x1300];
    u8 user[0x20];                     /* 0x1300: the parent's user info */
    u8 pad_1320[0x1524 - 0x1320];
    u8 fileCount;                      /* 0x1524 */
    u8 pad_1525[0x1788 - 0x1525];
    DlPlayOfferedFileOld files[DLPLAY_OFFER_SLOTS_OLD];    /* 0x1788 */
    u8 pad_74c8[0x74e0 - 0x74c8];
    u8 taskThreadStack[DLPLAY_WORKER_STACK_BYTES];   /* 0x74e0 */
    u8 currentTask[0x20];              /* 0x7ce0 */
} DlPlayHostLinkAreaOld;

/* One piece of the downloaded program kept in memory, found by its address in
 * the program. state 0 is an empty slot; 2 and above can be read. */
typedef struct DlPlayProgramChunk {
    u32 address;
    u32 size;
    u8 *data;
    u32 state;
} DlPlayProgramChunk;

#define DLPLAY_KEPT_CHUNK_SLOTS 4

typedef struct DlPlayProgramChunkSet {
    u32 unk_00;                        /* set to 1 after a read */
    u8 pad_04[0x30 - 0x04];
    DlPlayProgramChunk segments[DLPLAY_KEPT_CHUNK_SLOTS];
} DlPlayProgramChunkSet;

/* The task thread's work area, behind data_022acf00: an OSThread, then the
 * queue of tasks it runs. */
#define DLPLAY_WORKER_STACK_AT 0xb8

typedef struct DlPlayWorkerArea {
    u8 pad_00[0x94];
    u32 unk_94;                        /* cleared when the thread starts */
    u8 queue[0x20];
    u8 stack[1];                       /* the thread's stack runs to the end of the work area */
} DlPlayWorkerArea;

/* Download Play comm layer, parent side. */
#define DLPLAY_OFFER_SLOTS 16
#define DLPLAY_NOTHING_TRANSMITTED 21           /* one past the last WM error code: nothing was sent */

/* commState values, one per child. */
#define DLPLAY_CHILD_UNUSED 0
#define DLPLAY_CHILD_LINK_READY 1
#define DLPLAY_CHILD_JOINED 2
#define DLPLAY_CHILD_GONE 3
#define DLPLAY_CHILD_REFUSED 4
#define DLPLAY_CHILD_ADMITTED 5
#define DLPLAY_CHILD_RECEIVING 6
#define DLPLAY_CHILD_HAS_PROGRAM 7
#define DLPLAY_CHILD_TOLD_TO_BOOT 8
#define DLPLAY_CHILD_CAN_BOOT 9
#define DLPLAY_CHILD_ASKED_FILE 10
#define DLPLAY_CHILD_NO_ROOM 11
#define DLPLAY_CHILD_FINISHED 12
#define DLPLAY_CHILD_FAULT 13
#define DLPLAY_CHILD_AWAITING_START 14

/* Error codes sent with DLPLAY_CHILD_FAULT. */
#define DLPLAY_FAULT_RADIO_CALL 8
#define DLPLAY_FAULT_UNRECOVERABLE 9

#define DLPLAY_NO_FILE_CHOSEN (-1)               /* childFiles value: no file picked */

/* Message types the parent sends. */
#define DLPLAY_TO_CHILD_EMPTY 0
#define DLPLAY_TO_CHILD_UPLOAD_BEGINS 1
#define DLPLAY_TO_CHILD_REFUSE 2
#define DLPLAY_TO_CHILD_LAYOUT 3
#define DLPLAY_TO_CHILD_BLOCK 4
#define DLPLAY_TO_CHILD_START_PROGRAM 5
#define DLPLAY_TO_CHILD_NO_ROOM 6

/* Message types a child sends. */
#define DLPLAY_FROM_CHILD_ASK_FILE 7
#define DLPLAY_FROM_CHILD_GOT_LAYOUT 8
#define DLPLAY_FROM_CHILD_NEXT_BLOCK 9
#define DLPLAY_FROM_CHILD_HALT 10
#define DLPLAY_FROM_CHILD_WILL_START 11

/* What the application answers to a child (MbComm_Respond). */
#define DLPLAY_ANSWER_TURN_AWAY 0
#define DLPLAY_ANSWER_ADMIT 1
#define DLPLAY_ANSWER_BEGIN_UPLOAD 2
#define DLPLAY_ANSWER_START_PROGRAM 3

/* childRequest values: what the application asked for a child. */
#define DLPLAY_APP_WANTS_NOTHING 0
#define DLPLAY_APP_WANTS_UPLOAD 2
#define DLPLAY_APP_WANTS_ADMIT 3
#define DLPLAY_APP_WANTS_REFUSE 4
#define DLPLAY_APP_WANTS_START 5

#define DLPLAY_LAYOUT_SENT_BYTES 0xe4      /* bytes of a file's download info sent to children */
#define DLPLAY_HOST_MSG_HEAD_BYTES 6

/* The header of every parent message. */
typedef struct DlPlayHostMsgHead {
    u8 type;
    u8 pad_01;
    u16 fileId;
    u16 block;
} DlPlayHostMsgHead;

/* The header of a child message. */
typedef struct DlPlayChildMsgHead {
    u8 type;
    u8 pad_01;
    u16 request;                       /* block request for DLPLAY_FROM_CHILD_NEXT_BLOCK */
    u8 pad_04[0x14 - 4];
} DlPlayChildMsgHead;

typedef struct DlPlayPlayerBadge {
    u8 favoriteColor : 4;
    u8 playerNo : 4;
    u8 nameLength;
    u16 name[10];
} DlPlayPlayerBadge;

/* What a child sends with DLPLAY_FROM_CHILD_ASK_FILE. */
#define DLPLAY_FILE_ASK_BYTES 29

typedef struct DlPlayChildFileAsk {
    u32 ggid;
    DlPlayPlayerBadge user;
    u16 version;
    u8 fileId;
    u8 pad_1d[3];
} DlPlayChildFileAsk;

/* Where one block of a file is. */
/* A child's request (DLPLAY_FILE_ASK_BYTES bytes) arrives in pieces that each
 * fit one message. The parent collects them per child in DlPlayFragmentStore. */
#define DLPLAY_FRAGMENT_SLOT_BYTES 32
#define DLPLAY_FRAGMENT_TOTAL_BYTES 30

typedef struct DlPlayFragmentStore {
    u8 data[DLPLAY_CHILD_SLOTS][DLPLAY_FRAGMENT_SLOT_BYTES];
    u32 received[DLPLAY_CHILD_SLOTS];      /* bit n set once piece n of a child arrived */
} DlPlayFragmentStore;

typedef struct DlPlayFragmentGeometry {
    int pieceSize;                     /* bytes of data in one piece */
    int pieceCount;
    int dataSize;
} DlPlayFragmentGeometry;

/* A child message that carries one piece (type 7). */
typedef struct DlPlayRequestFragment {
    u8 type;
    u8 pad_01;
    u8 index;
    u8 data[1];
} DlPlayRequestFragment;

typedef struct DlPlayBlockSpot {
    u32 unk_00;
    u32 size;
    u32 offset;                        /* offset in the file */
    u8 segment;
    u8 pad_0d[3];
} DlPlayBlockSpot;

#define DLPLAY_HELD_BLOCK_SLOTS 4
#define DLPLAY_HELD_BLOCK_USABLE 2

typedef struct DlPlayHeldBlock {
    u32 address;                       /* offset in the ROM */
    u32 size;
    void *buffer;
    int state;
} DlPlayHeldBlock;

/* Pages of a file read from the card on demand. */
typedef struct DlPlayHeldProgram {
    u32 loadPending;                   /* cleared when a page load starts */
    u8 pad_04[0x10 - 4];
    char archiveName[4];
    int archiveNameLength;
    u8 pad_18[0x30 - 0x18];
    DlPlayHeldBlock pages[DLPLAY_HELD_BLOCK_SLOTS];
} DlPlayHeldProgram;

typedef struct DlPlayWorkerJob {
    u8 pad_00[4];
    u32 loading : 1;                   /* a page load is queued or running */
    u32 unk_04_1 : 31;
    u8 pad_08[8];
    DlPlayHeldBlock *page;
    DlPlayHeldProgram *cache;
} DlPlayWorkerJob;

/* One file the parent offers. */
typedef struct DlPlayOfferedFile {
    u8 dlFileInfo[DLPLAY_LAYOUT_SENT_BYTES];
    DlPlayGameEntry gameInfo;               /* the beacon entry for this file */
    u32 segmentOffsets[5];             /* block table: where each segment starts in the file */
    DlPlayGameSource *registry;
    u8 pad_5bc[4];
    u16 currentBlock;
    u16 nextBlock;
    u16 pollBitmap;                    /* children receiving this file */
    u16 childBitmap;                   /* children that picked this file */
    u16 changedBitmap;
    u8 active;
    u8 pad_5cb;
    DlPlayHeldProgram *cache;
    u32 *segmentAddresses;
} DlPlayOfferedFile;

typedef void (*DlPlayLinkEventFunc)(u32 aid, u32 state, void *arg);

/* The comm work area behind data_022aced4. Child arrays are indexed by
 * aid - 1 (aid 0 is the parent). */
typedef struct DlPlayHostLinkArea {
    u8 sendBuffer[4];                  /* length unknown */
    u8 pad_04[0x1340 - 4];
    DlPlayPlayerBadge childUsers[DLPLAY_CHILD_SLOTS];
    u16 childVersions[DLPLAY_CHILD_SLOTS];
    u32 childGgids[DLPLAY_CHILD_SLOTS];
    DlPlayLinkEventFunc callback;           /* application callback for comm events */
    int commState[DLPLAY_CHILD_SLOTS];
    u8 fileCount;
    u8 currentFile;
    u8 childFiles[DLPLAY_CHILD_SLOTS];     /* file each child picked */
    u8 childCount;
    u16 childEntryBitmap;
    u8 pad_1538[0x1754 - 0x1538];
    u16 childRequests[DLPLAY_CHILD_SLOTS];
    DlPlayPlayerBadge userInfoCopy;           /* filled by MbComm_GetChildUser */
    DlPlayOfferedFile files[DLPLAY_OFFER_SLOTS];
    u8 pad_74c8[0x7ce0 - 0x74c8];
    DlPlayWorkerJob task;
    u8 pad_7cf8[0x7d00 - 0x7cf8];
} DlPlayHostLinkArea;

int MbComm_IsChildAid(u32 aid);
int MbComm_Respond(u32 aid, int response);
void MbComm_SetCallback(DlPlayLinkEventFunc callback);
DlPlayPlayerBadge *MbComm_GetChildUser(u16 aid);
void MbComm_NotifyState(u32 aid, u32 state, void *arg);

/* A file of the comm work area. A macro, not a function: to match, the
 * address is rebuilt at each use as (work + index * file size)->files[0],
 * not as work->files[index]. */
#define MbComm_GetFile(work, fileIndex) \
    (((DlPlayHostLinkArea *)((u8 *)(work) + (fileIndex) * sizeof(DlPlayOfferedFile)))->files)

#endif
