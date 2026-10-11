#ifndef GAME_SOUND_HANDLE_H
#define GAME_SOUND_HANDLE_H

#include "types.h"
#include "game/link_list.h"

/* Ramps a value to a target over a number of frames (SNDi fader). */
typedef struct SoundFader {
    s32 start;
    s32 target;
    s32 elapsed;
    s32 frames;
} SoundFader;

/* SoundSeqPlayer.state */
#define SEQ_PLAYER_FREE     0
#define SEQ_PLAYER_PLAYING  1
#define SEQ_PLAYER_STOPPING 2  /* fading out, stops when the fader ends */

/* SoundSeqPlayer.sourceKind: how the playing sequence was started. */
#define SEQ_SOURCE_NONE    0
#define SEQ_SOURCE_SEQ     1   /* sourceNo is the sequence number */
#define SEQ_SOURCE_SEQ_ARC 2   /* sourceNo is the sequence archive, sourceIndex the entry in it */

struct SoundPlayerHeap;

/* One of the sequence player groups of the sound system, 0x24 bytes each
 * (array at data_022a84a8). A group holds the sequence players started on
 * it, sorted by priority, and has its own volume. */
typedef struct SoundPlayerSlot {
    LinkList players;                  /* 0x00 */
    LinkList heaps;                    /* 0x0c: SoundPlayerHeaps not in use by a player */
    u32 maxPlayers;                    /* 0x18 */
    u32 trackChannelMask;              /* 0x1c: hardware channels the tracks of the group may use, 0 for no limit */
    u8 volume;                         /* 0x20: index into the volume table */
    u8 pad_21[3];
} SoundPlayerSlot;

/* NitroSystem sequence player (NNSSndSeqPlayer), only the fields the game
 * code reads. 0x44 bytes each. */
typedef struct SoundSeqPlayer {
    struct SoundHandle *handle;        /* 0x00: handle that points at this player, or NULL */
    SoundPlayerSlot *ownerSlot;        /* 0x04: group this player sits in, or NULL */
    struct SoundPlayerHeap *heap;      /* 0x08: heap the player loads its data into, or NULL */
    ListLink slotLink;                 /* 0x0c: link in ownerSlot->players */
    ListLink activeLink;               /* 0x14: link in the list of all players (free or in use) */
    SoundFader volumeFader;            /* 0x1c */
    u8 state;                          /* 0x2c */
    u8 started;                        /* 0x2d: set once the first note has begun */
    u8 unk_2e;
    u8 pad_2f;
    u32 startTick;                     /* 0x30: tick passed to the has-passed check */
    u16 sourceKind;                    /* 0x34: SEQ_SOURCE_* */
    u8 pad_36[2];
    u16 sourceNo;                      /* 0x38 */
    u16 sourceIndex;                   /* 0x3a */
    u8 hwChannel;                      /* 0x3c: hardware channel number */
    u8 priority;                       /* 0x3d: the player lists are sorted by it */
    s16 hwVolume;                      /* 0x3e: last volume sent to the hardware channel */
    u8 seqVolume;                      /* 0x40: volume of the archive entry */
    u8 volume;                         /* 0x41: set by NNS_SndPlayerSetVolume (NNS_SndPlayerSetVolume) */
    u8 pad_42[2];
} SoundSeqPlayer;

/* A sound heap kept for a sequence player, 0x14 bytes. Sits in
 * SoundPlayerSlot.heaps while no player uses it; slotNo says which group. */
typedef struct SoundPlayerHeap {
    ListLink link;                     /* 0x00: link in SoundPlayerSlot.heaps */
    struct SoundHeap *heap;            /* 0x08: the sound heap */
    SoundSeqPlayer *player;            /* 0x0c: player using it, or NULL */
    u32 slotNo;                        /* 0x10: index of the group it belongs to */
} SoundPlayerHeap;

/* NitroSystem sound handle (NNSSndHandle): the player a sequence was
 * started on, NULL when nothing plays on it. */
typedef struct SoundHandle {
    SoundSeqPlayer *player;
} SoundHandle;

/* Stops player now, or fades it out over fadeFrames frames. */
void SoundSeqPlayer_Stop(SoundSeqPlayer *player, int fadeFrames);

#endif
