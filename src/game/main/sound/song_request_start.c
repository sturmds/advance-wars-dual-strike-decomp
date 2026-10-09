#include "types.h"
#include "game/song.h"

void func_020267a4(int songId);

/* Last step of the song request script: starts the requested song. */
void SongRequest_Start(SongRequestTask *task)
{
    func_020267a4(task->songId);
}
