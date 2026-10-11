#include "sdk/snd.h"

void Sound_PostCommandArgs(u32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    SndCommand *command = func_021104b8(1);

    if (command == NULL) {
        return;
    }
    command->id = id;
    command->args[0] = arg0;
    command->args[1] = arg1;
    command->args[2] = arg2;
    command->args[3] = arg3;
    Sound_LinkCommandAtTail(command);
}
