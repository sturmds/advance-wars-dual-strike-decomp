#include "types.h"

void func_020266dc(int length);   /* fades the song out over length * 16 frames */

/* Fades the song out over 32 frames. */
void Sound_FadeOutSong(void)
{
    func_020266dc(2);
}
