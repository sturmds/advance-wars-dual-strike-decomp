#include "types.h"
#include "game/song.h"

/* Reached only through pointers in data, next to Sound_RestoreSongVolume. */
void Sound_DimSong(void) { Sound_LowerSongVolume(80); }
