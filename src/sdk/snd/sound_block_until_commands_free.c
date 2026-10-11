#include "sdk/snd.h"

int func_0211005c(void);
int func_02110554(int block);
int func_021102b0(int flags);

/* Blocks until count commands are free. If the commands in flight would cover it, wait for the ARM7 to return them; otherwise send the queued ones first. */
void Sound_BlockUntilCommandsFree(int count)
{
    int freeCount = Sound_FreeCommandCount();

    if (freeCount >= count) {
        return;
    }
    if (freeCount + func_0211005c() >= count) {
        while (func_02110554(0) != 0) {
        }
        if (Sound_FreeCommandCount() >= count) {
            return;
        }
    } else {
        func_021102b0(1);
    }
    Sound_PingArm7Driver();
    do {
        func_02110554(1);
    } while (Sound_FreeCommandCount() < count);
}
