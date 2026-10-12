#include "types.h"

/* Song ids, from romhacking.net doc 890 (RadioTails). */
#define SONG_BLACK_HOLE_CO_POWER 0x1c
#define SONG_CO_POWER 0x1d

void Sound_PlaySong(int song);
BOOL Player_IsAlliedNations(int player);

/* Plays the CO Power theme for a player: the normal one when
 * Player_IsAlliedNations returns TRUE, else the Black Hole one. */
void PlayCoPowerTheme(int player) {
  if (Player_IsAlliedNations(player) == TRUE) {
    Sound_PlaySong(SONG_CO_POWER);
  } else {
    Sound_PlaySong(SONG_BLACK_HOLE_CO_POWER);
  }
}
