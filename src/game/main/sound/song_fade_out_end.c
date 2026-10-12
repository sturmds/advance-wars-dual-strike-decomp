#include "types.h"
#include "game/song.h"

/* Argument of opcode 0x11 in gSongFadeOutScript, probably run when the
 * task ends: after the fade-out no song plays. */
void SongFadeOut_End(void) { gCurrentSong = SONG_NONE; }
