#include "types.h"

int Battle_RunSetupHooks(void);
int Map_LoadPredeployedUnits(int);
int Battle_RegisterFrameCallbacks(int);
int func_020e88f8(int);

int func_020d3bf8(void)
{
    int t0 = Battle_RunSetupHooks();
    int t1 = Map_LoadPredeployedUnits(t0);
    int t2 = Battle_RegisterFrameCallbacks(t1);
    return func_020e88f8(t2);
}
