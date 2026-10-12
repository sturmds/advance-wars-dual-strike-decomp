#include "types.h"
#include "game/song.h"

void Sound_StopMusic(void); /* stops the song's sequence player */

void Sound_StopSong(void) {
  gCurrentSong = SONG_NONE;
  Sound_StopMusic();
}
