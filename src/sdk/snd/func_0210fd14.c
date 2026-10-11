#include "sdk/snd.h"

u32 func_02110800(u32 a, u32 b, u32 c);

void func_0210fd14(u32 a, u32 b, u32 c, u32 d, u32 e)
{
    Sound_PostCommandArgs(16, a, b, c, func_02110800(a, d, e));
}
