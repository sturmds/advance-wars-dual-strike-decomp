#include "types.h"
#include "game/song.h"

u16 gCurrentSong; /* song playing, SONG_NONE: none */

/* Sets the song volume to from (-1: keep the current volume), then fades
 * it to to over frames frames. */
void Sound_FadeMusicVolume(int from, int to, int frames);

/* Fades the song back to full volume (127) over 40 frames. */
void Sound_RestoreSongVolume(void) {
  if (gCurrentSong == SONG_NONE) {
    return;
  }
  Sound_FadeMusicVolume(-1, 127, 40);
}
