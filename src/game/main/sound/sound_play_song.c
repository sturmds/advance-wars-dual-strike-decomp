#include "types.h"
#include "game/Unk_02080440.h"

void func_02026768(int songId);   /* asks the music task to switch to songId unless it plays already */

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_0229c670;         /* song playing, 0xffff: none */
extern u16 data_0229c674;         /* song to start later, 0xffff: none */

/* Starts song songId. While unk_83 of the current state is 0 the song
 * is only kept as pending, and func_02026560 starts it later. */
void Sound_PlaySong(int songId)
{
    if (data_027c027c->unk_48[data_027c0284]->unk_83 != 0) {
        func_02026768(songId);
        return;
    }
    data_0229c674 = songId;
    data_0229c670 = 0xffff;
}
