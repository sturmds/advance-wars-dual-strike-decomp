#include "types.h"
#include "game/song.h"

/* Sets the song volume to from (-1: keep the current volume), then fades
 * it to to over frames frames. */
void Sound_FadeMusicVolume(int from, int to, int frames);

/* Fades the song from full volume (127) down to volume over 40 frames. */
void Sound_LowerSongVolume(int volume) {
  if (gCurrentSong == SONG_NONE) {
    return;
  }
  Sound_FadeMusicVolume(127, volume, 40);
}
