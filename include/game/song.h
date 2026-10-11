#ifndef GAME_SONG_H
#define GAME_SONG_H

#include "types.h"
#include "game/task.h"

/* Background music state (src/game/main/sound/). */

#define SONG_NONE 0xffff

extern u16 gCurrentSong;         /* song playing, SONG_NONE: none */
extern u16 gPendingSong;         /* song to start later, SONG_NONE: none */

/* Task script that counts down while the song fades out, then clears
 * gCurrentSong. Pairs of (opcode, argument) words. */
extern const TaskScriptCmd gSongFadeOutScript[];

/* Task script of Sound_RequestSong's task. */
extern const TaskScriptCmd gSongRequestScript[];

/* The task that runs gSongFadeOutScript (a 0x6c-byte task). */
typedef struct SongFadeOutTask {
    u8 pad_00[0x64];
    s16 framesLeft;                    /* frames until the fade ends */
} SongFadeOutTask;

/* The task Sound_StartFadeOutTask creates: it fades the song out over
 * fadeLength * 16 frames (see Sound_FadeOutSongOver), waits for the fade to
 * end, then resumes its script unless holdScript is set. */
typedef struct SongFadeOutWaitTask {
    u8 pad_00[0x30];
    int unk_30;                        /* cleared when the fade has ended */
    int step;                          /* SongFadeOutWaitTask_Update state */
    u8 pad_38[0x44 - 0x38];
    int fadeLength;
    u8 pad_48[0x10];
    int holdScript;
    u8 pad_5c[0x10];
} SongFadeOutWaitTask;

/* The task Sound_StartSongSwitchTask creates: it fades the current song out
 * over fadeLength * 16 frames, waits for the fade to end, starts songId,
 * then resumes its script unless holdScript is set. */
typedef struct SongSwitchTask {
    u8 pad_00[0x30];
    int unk_30;                        /* cleared when the new song starts */
    int step;                          /* SongSwitchTask_Update state */
    u8 pad_38[0x44 - 0x38];
    int fadeLength;
    u8 pad_48[0x54 - 0x48];
    int songId;
    int holdScript;
    u8 pad_5c[0x10];
} SongSwitchTask;

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
void Sound_StartFadeOutTask(int holdScript, int fadeLength);
SongSwitchTask *Sound_StartSongSwitchTask(int songId, int holdScript, int fadeLength);
void SongSwitchTask_Update(SongSwitchTask *task);
void SongFadeOutWaitTask_Update(SongFadeOutWaitTask *task);

#endif
