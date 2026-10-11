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
    u8 pad_4a8[0x4b0 - 0x4a8];
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

/* The parent's work area, behind data_022aced8. Only the fields the beacon
 * and send code read are named. */
typedef struct DlPlayStationArea {
    u8 pad_00[0x0c];
    u16 unk_0c;
    u16 unk_0e;
    u8 pad_10[2];
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    u8 pad_18[0x508 - 0x18];
    void (*sendCallback)(void);        /* callback for the MP send */
    u8 sendQueued;                     /* set to 1 once MbParent_Send queued the data */
    u8 unk_50d;
    u8 pad_50e[0x51c - 0x50e];
    void (*errorCallback)(u32 type, void *arg);
    u8 pad_520[4];
    u16 unk_524;
    u16 unk_526;
    u16 unk_528;
    u8 pad_52a[2];
    u16 unk_52c;
} DlPlayStationArea;

/* What errorCallback receives for type DLPLAY_EVENT_RADIO_FAULT. */
#define DLPLAY_EVENT_RADIO_FAULT 0xff

typedef struct DlPlayFailedCall {
    u16 apiid;
    u16 errcode;
} DlPlayFailedCall;

/* Download Play comm layer, parent side. */
#define DLPLAY_OFFER_SLOTS 16
#define DLPLAY_NOTHING_TRANSMITTED 21           /* one past the last WM error code: nothing was sent */

/* commState values, one per child. */
#define DLPLAY_CHILD_JOINED 2
#define DLPLAY_CHILD_REFUSED 4
#define DLPLAY_CHILD_ADMITTED 5
#define DLPLAY_CHILD_RECEIVING 6
#define DLPLAY_CHILD_HAS_PROGRAM 7
#define DLPLAY_CHILD_TOLD_TO_BOOT 8
#define DLPLAY_CHILD_ASKED_FILE 10
#define DLPLAY_CHILD_NO_ROOM 11
#define DLPLAY_CHILD_AWAITING_START 14

/* Message types the parent sends. */
#define DLPLAY_TO_CHILD_EMPTY 0
#define DLPLAY_TO_CHILD_UPLOAD_BEGINS 1
#define DLPLAY_TO_CHILD_REFUSE 2
#define DLPLAY_TO_CHILD_LAYOUT 3
#define DLPLAY_TO_CHILD_BLOCK 4
#define DLPLAY_TO_CHILD_START_PROGRAM 5
#define DLPLAY_TO_CHILD_NO_ROOM 6

/* Message types a child sends. */
#define DLPLAY_FROM_CHILD_ASK_FILE 8
#define DLPLAY_FROM_CHILD_GOT_LAYOUT 9
#define DLPLAY_FROM_CHILD_NEXT_BLOCK 10
#define DLPLAY_FROM_CHILD_HALT 11

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
    u8 pad_06[2];
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
    u32 address;
    u8 pad_04[8];
    int state;
} DlPlayHeldBlock;

/* Pages of a file read from the card on demand. */
typedef struct DlPlayHeldProgram {
    u32 loadPending;                   /* cleared when a page load starts */
    u8 pad_04[0x30 - 4];
    DlPlayHeldBlock pages[DLPLAY_HELD_BLOCK_SLOTS];
} DlPlayHeldProgram;

typedef struct DlPlayWorkerJob {
    u8 pad_00[0x10];
    DlPlayHeldBlock *page;
    DlPlayHeldProgram *cache;
} DlPlayWorkerJob;

/* One file the parent offers. */
typedef struct DlPlayOfferedFile {
    u8 dlFileInfo[DLPLAY_LAYOUT_SENT_BYTES];
    u8 pad_0e4[0x5a4 - 0xe4];
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

/* The comm work area behind data_022aced4. Child arrays are indexed by
 * aid - 1 (aid 0 is the parent). */
typedef struct DlPlayHostLinkArea {
    u8 sendBuffer[4];                  /* length unknown */
    u8 pad_04[0x1340 - 4];
    DlPlayPlayerBadge childUsers[DLPLAY_CHILD_SLOTS];
    u16 childVersions[DLPLAY_CHILD_SLOTS];
    u32 childGgids[DLPLAY_CHILD_SLOTS];
    u8 pad_14e4[4];
    int commState[DLPLAY_CHILD_SLOTS];
    u8 fileCount;
    u8 currentFile;
    u8 childFiles[DLPLAY_CHILD_SLOTS];     /* file each child picked */
    u8 childCount;
    u16 childEntryBitmap;
    u8 pad_1538[0x1754 - 0x1538];
    u16 childRequests[DLPLAY_CHILD_SLOTS];
    u8 pad_1772[0x1788 - 0x1772];
    DlPlayOfferedFile files[DLPLAY_OFFER_SLOTS];
    u8 pad_74c8[0x7ce0 - 0x74c8];
    DlPlayWorkerJob task;
} DlPlayHostLinkArea;

#endif
