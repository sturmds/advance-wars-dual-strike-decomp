#include "types.h"

int WifiStation_Entry(int playerIndex, int slot, int powerState);

int func_arm7_027cb0e4(int playerIndex, int slot, int powerState)
{
    return WifiStation_Entry(playerIndex, slot, powerState) + 4;
}
