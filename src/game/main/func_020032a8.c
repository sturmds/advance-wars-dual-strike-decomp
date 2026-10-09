#include "types.h"

void Sprites_ResetLists(void);
int OamBuffer_Split(int);

int func_020032a8(void)
{
    Sprites_ResetLists();
    return OamBuffer_Split(16);
}
