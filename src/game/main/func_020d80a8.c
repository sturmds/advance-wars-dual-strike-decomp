#include "types.h"

u32 CallbackSlots_Clear(u32 co);
int Wallpaper_DestroyTasks(int exp);

int func_020d80a8(u32 co)
{
    return Wallpaper_DestroyTasks(CallbackSlots_Clear(co));
}
