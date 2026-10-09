#ifndef GAME_SONG_H
#define GAME_SONG_H

#include "types.h"

/* Background music state (src/game/main/sound/). */

#define SONG_NONE 0xffff

extern u16 gCurrentSong;         /* song playing, SONG_NONE: none */
extern u16 gPendingSong;         /* song to start later, SONG_NONE: none */

/* Task script that counts down while the song fades out, then clears
 * gCurrentSong. Pairs of (opcode, argument) words. */
extern const u32 gSongFadeOutScript[];

/* Task script of Sound_RequestSong's task. */
extern const u32 gSongRequestScript[];

/* The task that runs gSongFadeOutScript (a 0x6c-byte task). */
typedef struct SongFadeOutTask {
    u8 pad_00[0x64];
    s16 framesLeft;                    /* frames until the fade ends */
} SongFadeOutTask;

/* The task Sound_RequestSong creates (script gSongRequestScript): it waits
 * while Sound_IsSongFadingOut, then starts songId. */
typedef struct SongRequestTask {
    u8 pad_00[0x54];
    int songId;
} SongRequestTask;

void Sound_RestoreSongVolume(void);
void Sound_LowerSongVolume(int volume);
void Sound_DimSong(void);                /* Sound_LowerSongVolume(80) */
BOOL Sound_IsSongFadingOut(void);
void Sound_FadeOutSong(void);
void Sound_FadeOutSongOver(int length);  /* over length * 16 frames; 0: 2 */
void Sound_StopSong(void);
void Sound_StartSong(int songId);

#endif
