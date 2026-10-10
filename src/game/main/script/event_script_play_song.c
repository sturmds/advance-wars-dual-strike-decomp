#include "types.h"
#include "game/Unk_0201d320.h"

extern u8 data_022900b8;
extern Unk_0201d320 data_022977f4[];
void Sound_RequestSong(int songId);

s16 EventScript_PlaySong(s16 index)
{
    Unk_0201d320 *script = &data_022977f4[index];
    BOOL done;
    int songId = script->unk_00->unk_0c;

    /* when data_022900b8 is 1 the command only advances; no song is requested */
    if (data_022900b8 == 1) {
        done = 1;
    } else {
        Sound_RequestSong(songId);
        done = 0;
    }
    script->unk_00++;
    return done;
}
