#ifndef GAME_SONG_H
#define GAME_SONG_H

#include "types.h"

/* Background music state (src/game/main/sound/). */

#define SONG_NONE 0xffff

extern u16 gCurrentSong;         /* song playing, SONG_NONE: none */
extern u16 gPendingSong;         /* song to start later, SONG_NONE: none */

void Sound_RestoreSongVolume(void);
void Sound_LowerSongVolume(int volume);
void Sound_DimSong(void);                /* Sound_LowerSongVolume(80) */

#endif
