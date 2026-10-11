#include "types.h"

void Sound_StartSong(u32 songId);
BOOL func_02038064(void);

void func_02032984(void)
{
    if (func_02038064()) {
        Sound_StartSong(21);
    } else {
        Sound_StartSong(20);
    }
}
