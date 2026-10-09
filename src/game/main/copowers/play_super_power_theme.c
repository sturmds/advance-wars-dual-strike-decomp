#include "types.h"

/* Song ids, from romhacking.net doc 890 (RadioTails). */
#define SONG_SUPER_POWER            0x1f
#define SONG_BLACK_HOLE_SUPER_POWER 0x20

void Sound_PlaySong(int song);
BOOL func_020e3110(int player);

/* Plays the Super CO Power theme for a player: the normal one when
 * func_020e3110 returns TRUE (an Allied Nations CO, per doc 890), else
 * the Black Hole one. */
void PlaySuperPowerTheme(int player)
{
    if (func_020e3110(player) == TRUE) {
        Sound_PlaySong(SONG_SUPER_POWER);
    } else {
        Sound_PlaySong(SONG_BLACK_HOLE_SUPER_POWER);
    }
}
