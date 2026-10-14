#ifndef SDK_MB_H
#define SDK_MB_H

#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

/* Download Play (MB) parent side: the games the parent offers, and the
 * beacon that advertises them. */

#define DLPLAY_ICON_COLOR_BYTES 0x20
#define DLPLAY_ICON_PIXEL_BYTES 0x200
#define DLPLAY_GAME_EXTRA_BYTES 22
#define DLPLAY_TITLE_CHARS 48
#define DLPLAY_BLURB_CHARS 96
#define DLPLAY_CHILD_SLOTS 15
#define DLPLAY_PLAYER_BADGE_BYTES 22

/* Bytes of the work area the parent hands DownloadPlay_SetUpWork. */
#define DLPLAY_WORK_SIZE 0xc000

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
  u8 playerCount; /* parent plus joined children */
  u8 pad_359;
  u16 playerFlags; /* bit 0 is the parent */
  u16 unk_35c;
  u8 users[DLPLAY_CHILD_SLOTS][DLPLAY_PLAYER_BADGE_BYTES];
  u8 unk_4a8[8];       /* sent at the end of the player beacon */
  u16 sentPlayerFlags; /* beacon progress: bit 0 once the game's pieces went out, bit n once player n's info did */
  u8 noIcon;           /* 1 when an icon file could not be read */
  u8 seqNo;            /* registration order, from gMbGameSequence */
  u8 changeCount;      /* up by one at each player change */
  u8 fileNo;           /* slot in DlPlayHostLinkArea.files */
  u8 pad_4b6[2];
  u32 ggid;
  struct DlPlayGameEntry* next; /* list link */
} DlPlayGameEntry;

/* What the parent is told about a game when it registers it. */
typedef struct DlPlayGameSource {
  const char* romFilePath;
  const u16* gameName;
  const u16* gameIntro;
  const char* iconCharPath;
  const char* iconPalettePath;
  u32 ggid;
  u8 maxPlayers;
} DlPlayGameSource;

/* Beacon state values. */
#define DLPLAY_ADVERT_STEP_OFF 0
#define DLPLAY_ADVERT_STEP_PICK 1  /* pick the next game */
#define DLPLAY_ADVERT_STEP_SPLIT 2 /* split the game into pieces */
#define DLPLAY_ADVERT_STEP_EMIT 3  /* send one piece per call */
#define DLPLAY_ADVERT_STEP_DONE 4
#define DLPLAY_ADVERT_STEP_ROSTER 5 /* send the player list */

/* data_022ace4c */
typedef struct DlPlayAdvertProgress {
  DlPlayGameEntry* gameList;
  DlPlayGameEntry* currentGame;
  u8* nextPiece; /* next byte of the game to send */
  u8 state;
  u8 unk_0d;
  u8 changeCount; /* copy of the game's changeCount */
  u8 pieceIndex;
  u8 pieceCount;
  u8 sequence;
} DlPlayAdvertProgress;

#define DLPLAY_ADVERT_CHUNK_BYTES 98
#define DLPLAY_ADVERT_SUMMED_BYTES \
  0x68 /* from the checksum field to the end of DlPlayAdvertFrame */

/* The user game info the parent broadcasts, 0x70 bytes at data_022ace60. */
typedef struct DlPlayAdvertFrame {
  u32 ggid;
  u8 noIcon : 2;
  u8 fileNo : 6;
  u8 seqNo;
  u8 changeCount;
  u8 sequence;  /* up by one at each beacon */
  u16 checksum; /* over the bytes from here to the end */
  u8 pieceIndex;
  u8 pieceCount;
  u8 size; /* bytes used in data */
  u8 pad_0d;
  u8 data[DLPLAY_ADVERT_CHUNK_BYTES];
} DlPlayAdvertFrame;

#define DLPLAY_ADVERT_IS_ROSTER 2 /* value of noIcon/kind in a player beacon */
#define DLPLAY_ROSTER_PER_FRAME 4

/* The same 0x70 bytes as DlPlayAdvertFrame, as sent in state DLPLAY_ADVERT_STEP_ROSTER:
 * up to four players' user info per beacon. */
typedef struct DlPlayRosterFrame {
  u32 ggid;
  u8 kind : 2; /* DLPLAY_ADVERT_IS_ROSTER */
  u8 fileNo : 6;
  u8 seqNo;
  u8 changeCount;
  u8 sequence;
  u16 checksum;
  u8 playerCount;
  u8 pad_0b;
  u16 playerFlags;
  u16 unk_35c;
  u8 users[DLPLAY_ROSTER_PER_FRAME]
          [DLPLAY_PLAYER_BADGE_BYTES]; /* a player number of 0 ends the list */
  u8 unk_4a8[8];
} DlPlayRosterFrame;

/* The beacon buffer at data_022ace60, in either form. */
typedef union DlPlayBeaconFrame {
  DlPlayAdvertFrame advert;
  DlPlayRosterFrame roster;
} DlPlayBeaconFrame;

#define DLPLAY_SCAN_HIT_SLOTS 16

/* A parent the child side found by scanning: its beacon and the scan result. */
typedef struct DlPlayScannedHost {
  WMBssDesc bssDesc;
  WMStartScanCallback scan;
  u8 pad_178[8];
} DlPlayScannedHost;

/* The parent's work area, behind data_022aced8. Only the fields the beacon
 * and send code read are named. */
typedef struct DlPlayStationArea {
  u8 pad_00[0x08];
  u32 ggid;
  u16 unk_0c;
  u16 unk_0e;
  u16 maxChildren; /* set by DownloadPlay_SetSendSizeAndChildren */
  u16 unk_12;
  u16 unk_14;
  u16 unk_16;
  u16 beaconPeriod; /* milliseconds */
  u8 pad_1a[0x32 - 0x1a];
  u16 channel;
  u16 parentMaxSize; /* bytes in one parent MP packet */
  u16 childMaxSize;
  u8 pad_38[0x40 - 0x38];
  u16 mpSendBuffer[2]; /* 0x40: given to WM_StartMPEx, length unknown */
  u8 pad_44[0x320 - 0x44];
  u32 unk_320; /* 0 until the game starts Download Play */
  u8 pad_324[0x440 - 0x324];
  WMBssDesc scanBuffer; /* where WM writes a parent found by the scan */
  u16 sendSize;         /* set by DownloadPlay_SetSendSizeAndChildren */
  u16 unk_502;
  u16* mpRecvBuffer; /* given to WM_StartMPEx */
  WMCallbackFunc
      wmCallback; /* given to WM_Initialize, WM_SetIndCallback, WM_Reset and the MP send */
  u8 sendQueued; /* set to 1 once MbParent_Send queued the data */
  u8 started;    /* 1 from MbParent_InitWm until WM ends */
  u8 pad_50e[0x518 - 0x50e];
  u16 mpSendSize; /* sizes given to WM_StartMPEx */
  u16 mpRecvSize;
  void (*callback)(
      u32 type,
      void* arg); /* gets every DLPLAY_EVENT_* event, not only errors */
  WMBssDesc* connectBssDesc; /* the parent the child connects to */
  u16 unk_524;
  u16 unk_526;
  u16 unk_528;
  u16 unk_52a;
  u16 unk_52c;
  u8 pad_52e[2];
  u16 userName[4]; /* DownloadPlay_SetUpWork copies the user's nickname here */
  u16 unk_538
      [8]; /* DownloadPlay_SetUpWork copies L"multiboot" here, up to 16 characters */
  u16 unk_548;
  u8 pad_54a[0x5e0 - 0x54a];
  u16 foundCount;      /* entries used in found */
  u16 aid;             /* the id the parent gave this child */
  u32 unk_5e4;         /* nonzero: scan again after each scan result */
  u32 scanAllChannels; /* move to the next channel after each scan result */
  u32 lastFoundIndex;  /* entry of found the last scan result went to */
  u8 pad_5f0[0x600 - 0x5f0];
  DlPlayScannedHost found[DLPLAY_SCAN_HIT_SLOTS];
} DlPlayStationArea;

/* data_022aced4 points to the MB buffer; the parent work area sits 0x1000
 * bytes into it, which is what data_022aced8 points to. */
typedef struct DlPlayBufferArea {
  u8 pad_000[0x1000];
  DlPlayStationArea parent;
} DlPlayBufferArea;

/* Return codes of the MB calls. */
#define DLPLAY_RESULT_OK 0
#define DLPLAY_RESULT_WRONG_PHASE 2

/* Event types of the parent's callbacks (DlPlayStationArea.callback, MbParent_CommCallback). */
#define DLPLAY_EVENT_CHILD_JOINED 0
#define DLPLAY_EVENT_CHILD_LEFT 1
#define DLPLAY_EVENT_HOST_FRAME_OUT 2
#define DLPLAY_EVENT_HOST_FRAME_IN 3
#define DLPLAY_EVENT_SHUTDOWN_DONE 17
#define DLPLAY_EVENT_TRANSMIT_FAILED 19
#define DLPLAY_EVENT_HOST_SETTINGS_DONE 21
#define DLPLAY_EVENT_STARTUP_DONE 21
#define DLPLAY_EVENT_MAY_TRANSMIT 25
#define DLPLAY_EVENT_BEACON_OUT 28
#define DLPLAY_EVENT_HEARD_BEACON 29
#define DLPLAY_EVENT_STATION_DROPPED 31
#define DLPLAY_EVENT_STATION_REJOINED 32
#define DLPLAY_EVENT_STATION_AUTHED 33
#define DLPLAY_EVENT_OUTBOX_FULL 42
#define DLPLAY_EVENT_RADIO_FAULT \
  0x100 /* a WM call failed or sent an unexpected state; the argument is its WMCallback */
#define DLPLAY_EVENT_CALL_REFUSED \
  0xff /* a WM call returned an error; the argument is an DlPlayFailedCall */
#define DLPLAY_EVENT_PORT_DATA 9 /* data arrived on the MB port */

/* More callback types, as the child side's WM callback sends them. The
 * names are a best guess from the order the child-side callback sends them. */
#define DLPLAY_EVENT_SCAN_HIT 4
#define DLPLAY_EVENT_SCAN_MISS 5
#define DLPLAY_EVENT_JOINED_HOST 6
#define DLPLAY_EVENT_CHILD_FRAME_OUT 8
#define DLPLAY_EVENT_LOST_HOST 10
#define DLPLAY_EVENT_JOIN_REFUSED 11
#define DLPLAY_EVENT_CHILD_FRAME_FAILED 18
#define DLPLAY_EVENT_CHILD_FRAME_LATE 41

#define DLPLAY_WIRELESS_PORT 1 /* WM port of the MB data */

typedef struct DlPlayFailedCall {
  u16 apiid;
  u16 errcode;
} DlPlayFailedCall;

/* Where a child program's segments lie in the ROM, 16 bytes, in download
 * segment order (see gMbSegmentTypes). */
typedef struct DlPlayProgramRomOffsets {
  u32 headerRomAddress; /* 0: the ROM header starts the file */
  u32 arm9RomAddress;
  u32 arm7RomAddress;
  u32 unk_0c;
} DlPlayProgramRomOffsets;

/* How DownloadPlay_LoadChildProgram copies part of the running program's ARM9 code into
 * the buffer it sends: ROM offset x comes from source + x and goes to
 * dest + x, for x in [romOffset, romOffset + length). */
typedef struct DlPlayLiveCodePatch {
  u32 romOffset;
  const u8* source;
  u8* dest;
  u32 length;
} DlPlayLiveCodePatch;

/* The cartridge's secure area, ROM offsets 0x4000 to 0x8000. */
#define DLPLAY_PROTECTED_ROM_START 0x4000
#define DLPLAY_PROTECTED_ROM_END 0x8000

/* Sizes DownloadPlay_LoadChildProgram uses when it splits the buffer. */
#define DLPLAY_SIGNATURE_BYTES 0x88
#define DLPLAY_ASSUMED_CART_BYTES \
  0x1000000 /* when the header gives no ROM size */
#define DLPLAY_MAINCPU_SPLIT_BYTES \
  0x4400 /* ARM9 code goes in three parts when both programs do not fit */
#define DLPLAY_RETURN_OPCODE 0xe12fff1e /* ARM "bx lr" */
#define DLPLAY_PREFIX_BYTES 0x268 /* header, auth code, cache and card map */
#define DLPLAY_PROGRAM_BUFFER_FLOOR 0x10000

/* Offsets into the buffer given with a file. */
#define DLPLAY_SOURCE_HELD_AT 0x1e8
#define DLPLAY_SOURCE_ROM_OFFSETS_AT 0x258

#define DLPLAY_WORKER_STACK_BYTES 0x800

/* Download Play comm layer, parent side. */
#define DLPLAY_OFFER_SLOTS 16
#define DLPLAY_NOTHING_TRANSMITTED \
  21 /* one past the last WM error code: nothing was sent */

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

#define DLPLAY_NO_FILE_CHOSEN (-1) /* childFiles value: no file picked */

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

#define DLPLAY_LAYOUT_SENT_BYTES \
  0xe4 /* bytes of a file's download info sent to children */
#define DLPLAY_HOST_MSG_HEAD_BYTES 6

/* The header of every parent message. */
typedef struct DlPlayHostMsgHead {
  u8 type;
  u8 pad_01;
  u16 fileId;
  u16 block;
} DlPlayHostMsgHead;

/* A child message as received: the type, the request halfword as two
   bytes (it is not aligned), then the payload. */
typedef struct DlPlayChildMsgWire {
  u8 type;
  u8 request[2];
  u8 payload[1];
} DlPlayChildMsgWire;

/* The header of a child message. */
typedef struct DlPlayChildMsgHead {
  u8 type;
  u8 pad_01;
  u16 request; /* block request for DLPLAY_FROM_CHILD_NEXT_BLOCK */
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
  u32 received
      [DLPLAY_CHILD_SLOTS]; /* bit n set once piece n of a child arrived */
} DlPlayFragmentStore;

typedef struct DlPlayFragmentGeometry {
  int pieceSize; /* bytes of data in one piece */
  int pieceCount;
  int dataSize;
} DlPlayFragmentGeometry;

extern DlPlayFragmentStore* gMbRecvPieces;
extern DlPlayFragmentGeometry gMbRecvPieceSize;

void MbPieces_Init(DlPlayFragmentStore* buffer);

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
  u32 offset; /* offset in the file */
  u8 segment;
  u8 pad_0d[3];
} DlPlayBlockSpot;

/* Kinds of program segment a child downloads (gMbSegmentTypes lists them in
 * download order). */
#define DLPLAY_PART_MAINCPU_CODE 0
#define DLPLAY_PART_SUBCPU_CODE 1
#define DLPLAY_PART_CART_HEADER 2
#define DLPLAY_PROGRAM_PART_COUNT 3

extern const u32 gMbSegmentTypes[DLPLAY_PROGRAM_PART_COUNT];

/* Where a file's segments start, in the file and in blocks, 20 bytes. */
typedef struct DlPlaySegmentStarts {
  u32 segmentOffsets
      [DLPLAY_PROGRAM_PART_COUNT]; /* where each segment starts in the file */
  u16 segmentFirstBlocks[DLPLAY_PROGRAM_PART_COUNT];
  u16 blockCount;
} DlPlaySegmentStarts;

typedef struct DlPlaySegmentPlacement {
  u32 recvAddress; /* where the child receives it */
  u32 loadAddress;
  u32 size;
  u32 target : 1; /* MI_PROCESSOR_ARM9 or MI_PROCESSOR_ARM7 */
  u32 reserved : 31;
} DlPlaySegmentPlacement;

#define DLPLAY_CART_HEADER_BYTES 0x160

/* A file's download info (DlPlayOfferedFile.dlFileInfo), as far as known. */
typedef struct DlPlayProgramLayout {
  u32 arm9EntryAddress;
  u32 arm7EntryAddress;
  u32 pad_08;
  DlPlaySegmentPlacement segments[DLPLAY_PROGRAM_PART_COUNT];
  u8 authCode[DLPLAY_SIGNATURE_BYTES];
} DlPlayProgramLayout;

/* Where each kind of segment may be received or loaded. */
#define DLPLAY_MAIN_RAM_BASE 0x02000000
#define DLPLAY_MAINCPU_CODE_AREA_END 0x022c0000
#define DLPLAY_SUBCPU_CODE_STAGING \
  DLPLAY_MAINCPU_CODE_AREA_END /* where a relocated ARM7 static is received */
#define DLPLAY_SUBCPU_CODE_AREA_END 0x02300000
#define DLPLAY_HOST_DESC_SLOT 0x023fe800
#define DLPLAY_SUBCPU_CODE_SIZE_LIMIT \
  0x40000 /* past DLPLAY_SUBCPU_CODE_AREA_END */
#define DLPLAY_HEADER_COPY_START 0x027ffe00
#define DLPLAY_HEADER_COPY_END 0x027fff60
#define DLPLAY_FAST_RAM_START 0x037f8000 /* shared WRAM, then ARM7 WRAM */
#define DLPLAY_FAST_RAM_END 0x0380f000

#define DLPLAY_HELD_BLOCK_SLOTS 4
#define DLPLAY_HELD_BLOCK_FILLING 1
#define DLPLAY_HELD_BLOCK_USABLE 2 /* this state and above can be read */
#define DLPLAY_HELD_BLOCK_PINNED 3 /* kept for the whole download */

/* One block of a child program kept in memory, 16 bytes. state 0 is a free
 * slot. */
typedef struct DlPlayHeldBlock {
  u32 address; /* offset in the ROM */
  u32 size;
  u8* buffer;
  u32 state; /* DLPLAY_HELD_BLOCK_* */
} DlPlayHeldBlock;

/* The blocks of one child program held in memory, 0x70 bytes: header, ARM9
 * static, ARM7 static (or ARM9 parts), plus pages read from the card on
 * demand. DownloadPlay_LoadChildProgram builds it in the file's buffer. */
typedef struct DlPlayHeldProgram {
  u32 readSinceLoad; /* set by each read that hits, cleared when a page load starts */
  u8 pad_04[0x10 - 4];
  char archiveName[4];
  int archiveNameLength;
  u8 pad_18[0x30 - 0x18];
  DlPlayHeldBlock pages[DLPLAY_HELD_BLOCK_SLOTS];
} DlPlayHeldProgram;

void MbCache_Init(DlPlayHeldProgram* cache);
void MbCache_Add(DlPlayHeldProgram* cache, u32 address, u32 size, u8* buffer,
                 u32 state);
int MbCache_Read(DlPlayHeldProgram* cache, u32 address, void* dst, u32 size);

#define DLPLAY_JOB_PRIO_BOTTOM 31 /* thread priorities 0 (highest) to 31 */
#define DLPLAY_JOB_PRIO_HIGHER 32 /* one above the task thread's priority */
#define DLPLAY_JOB_PRIO_LOWER 33  /* one below it */
#define DLPLAY_JOB_PRIO_SAME 34   /* the same */

/* A task for the task thread: task runs on the thread, then callback. */
typedef struct DlPlayWorkerJob {
  struct DlPlayWorkerJob* next;
  u32 busy : 1; /* queued or running */
  u32 priority
      : 31; /* thread priority, or DLPLAY_JOB_PRIO_HIGHER/BELOW/NORMAL */
  void (*task)(struct DlPlayWorkerJob* info);
  void (*callback)(struct DlPlayWorkerJob* info);
  DlPlayHeldBlock* page;
  DlPlayHeldProgram* cache;
  u8 pad_18[8];
} DlPlayWorkerJob;

/* The task thread's work area, behind data_022acf00: an OSThread, the list
 * of tasks waiting, the end task, then the thread's stack. */
#define DLPLAY_WORKER_STACK_AT 0xb8

typedef struct DlPlayWorkerArea {
  OSThread thread;
  DlPlayWorkerJob* volatile list; /* sorted by priority; volatile to match: each use reads it again */
  DlPlayWorkerJob
      endTask; /* posted by MbTask_End; runs last and stops the thread */
  u8 stack[1]; /* the thread's stack runs to the end of the work area */
} DlPlayWorkerArea;

BOOL MbTask_IsAvailable(void);
void MbTask_InitInfo(DlPlayWorkerJob* info);
void MbTask_Set(DlPlayWorkerJob* info, void (*task)(DlPlayWorkerJob* info),
                void (*callback)(DlPlayWorkerJob* info), u32 priority);
void MbTask_End(void (*callback)(DlPlayWorkerJob* info));
int MbTask_IsBusy(DlPlayWorkerJob* task);

/* One file the parent offers. */
typedef struct DlPlayOfferedFile {
  u8 dlFileInfo[DLPLAY_LAYOUT_SENT_BYTES];
  DlPlayGameEntry gameInfo; /* the beacon entry for this file */
  DlPlaySegmentStarts blockTable;
  const DlPlayGameSource* registry;
  const void*
      source; /* ROM header, auth code, cache and card map (DownloadPlay_LoadChildProgram) */
  u16 currentBlock;
  u16 nextBlock;
  u16 pollBitmap;  /* children receiving this file */
  u16 childBitmap; /* children that picked this file */
  u16 changedBitmap;
  u8 active;
  u8 pad_5cb;
  DlPlayHeldProgram* cache; /* in source, at DLPLAY_SOURCE_HELD_AT */
  u32*
      segmentRomAddresses; /* the DlPlayProgramRomOffsets in source, by download segment */
} DlPlayOfferedFile;

typedef void (*DlPlayLinkEventFunc)(u32 aid, u32 state, void* arg);

/* The comm work area behind data_022aced4. Child arrays are indexed by
 * aid - 1 (aid 0 is the parent). */
typedef struct DlPlayHostLinkArea {
  u8 sendBuffer[4]; /* length unknown */
  u8 pad_04[0x400 - 4];
  u16 unk_400[(0x1300 - 0x400) / 2];
  DlPlayPlayerBadge user; /* the parent's user info */
  u16 unk_1316;
  u32 blockSizeMax; /* bytes of file data in one parent message */
  u32 mpStarting;   /* 1 from WM_StartMPEx until its callback */
  u32 unk_1320;     /* DlPlayBufferArea.parent.unk_320 */
  u8 pad_1324[0x1340 - 0x1324];
  DlPlayPlayerBadge childUsers[DLPLAY_CHILD_SLOTS];
  u16 childVersions[DLPLAY_CHILD_SLOTS];
  u32 childGgids[DLPLAY_CHILD_SLOTS];
  DlPlayLinkEventFunc callback; /* application callback for comm events */
  int commState[DLPLAY_CHILD_SLOTS];
  u8 fileCount;
  u8 currentFile;
  s8 childFiles
      [DLPLAY_CHILD_SLOTS]; /* file each child picked, DLPLAY_NO_FILE_CHOSEN for none */
  u8 childCount;
  u16 childEntryBitmap;
  DlPlayFragmentStore
      pieceBuffer; /* request pieces from children (MbPieces_Init) */
  u16 childRequests[DLPLAY_CHILD_SLOTS];
  DlPlayPlayerBadge userInfoCopy; /* filled by MbComm_GetChildUser */
  DlPlayOfferedFile files[DLPLAY_OFFER_SLOTS];
  u32 unk_74c8;
  u8 pad_74cc[0x74e0 - 0x74cc];
  u8 taskThreadStack[DLPLAY_WORKER_STACK_BYTES];
  DlPlayWorkerJob task;
} DlPlayHostLinkArea;

int MbComm_IsChildAid(u32 aid);
int MbComm_Respond(u32 aid, int response);
void MbComm_SetCallback(DlPlayLinkEventFunc callback);
DlPlayPlayerBadge* MbComm_GetChildUser(u16 aid);
void MbComm_NotifyState(u32 aid, u32 state, void* arg);

/* A file of the comm work area. A macro, not a function: to match, the
 * address is rebuilt at each use as (work + index * file size)->files[0],
 * not as work->files[index]. */
#define MbComm_GetFile(work, fileIndex)                             \
  (((DlPlayHostLinkArea*)((u8*)(work) +                             \
                          (fileIndex) * sizeof(DlPlayOfferedFile))) \
       ->files)

/* Work areas of the parent: data_022aced4 points to the MB buffer
 * (DlPlayBufferArea's view puts the parent area 0x1000 bytes in, where
 * data_022aced8 points), data_022acf00 to the task thread's area. */
extern DlPlayHostLinkArea* data_022aced4;
extern DlPlayStationArea* data_022aced8;
extern DlPlayWorkerArea* data_022acf00;
extern void* data_022acedc; /* WM work buffer given to WM_Initialize */
extern u16 data_022aced0;   /* DMA channel given to WM_Initialize */

/* Beacon state: the game list and progress, the hook called with the ggid
 * around a player-list beacon, and the beacon buffer. */
extern DlPlayAdvertProgress data_022ace4c;
extern void (*data_022ace48)(u32 ggid);
extern DlPlayBeaconFrame data_022ace60;

/* The child's scan request (MbChild_WmCallback). */
extern WMScanParam data_022acee0;

#endif
