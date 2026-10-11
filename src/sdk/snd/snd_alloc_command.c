#include "sdk/snd.h"

int func_0211005c(void);
SndCommand *func_02110554(int block);
void SND_FlushCommand(int block);
int func_0210dc64(void);

SndCommand *SND_AllocCommand(int block)
{
    SndCommand *command;

    if (func_0210dc64() != 0) {
        return NULL;
    }
    command = SND_PopFreeCommand();
    if (command != NULL) {
        return command;
    }
    if (!(block & 1)) {
        return NULL;
    }
    if (func_0211005c() > 0) {
        while (func_02110554(0) != NULL) {
        }
        command = SND_PopFreeCommand();
        if (command != NULL) {
            return command;
        }
    } else {
        SND_FlushCommand(1);
    }
    Sound_PingArm7Driver();
    do {
        func_02110554(1);
        command = SND_PopFreeCommand();
    } while (command == NULL);
    return command;
}
