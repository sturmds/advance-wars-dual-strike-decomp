#include "types.h"

/* Song ids, from romhacking.net doc 890 (RadioTails). */
#define SONG_TAG_POWER 0x2e
#define SONG_BLACK_HOLE_TAG_POWER 0x2f

void Sound_PlaySong(int song);
BOOL Player_IsAlliedNations(int player);

/* Plays the Tag CO Power theme for a player: the normal one when
 * Player_IsAlliedNations returns TRUE, else the Black Hole one. */
void PlayTagPowerTheme(int player) {
  if (Player_IsAlliedNations(player) == TRUE) {
    Sound_PlaySong(SONG_TAG_POWER);
  } else {
    Sound_PlaySong(SONG_BLACK_HOLE_TAG_POWER);
  }
}
