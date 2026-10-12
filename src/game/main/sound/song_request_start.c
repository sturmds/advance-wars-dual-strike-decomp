#include "types.h"
#include "game/song.h"

/* Last step of the song request script: starts the requested song. */
void SongRequest_Start(SongRequestTask* task) { Sound_StartSong(task->songId); }
