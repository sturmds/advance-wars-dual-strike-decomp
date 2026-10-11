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

#endif
