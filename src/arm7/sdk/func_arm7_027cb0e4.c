#include "types.h"

int func_arm7_027cb030(int playerIndex, int slot, int powerState);

int func_arm7_027cb0e4(int playerIndex, int slot, int powerState)
{
    return func_arm7_027cb030(playerIndex, slot, powerState) + 4;
}
