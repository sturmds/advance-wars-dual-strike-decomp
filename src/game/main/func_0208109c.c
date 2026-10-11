#include "types.h"
#include "game/link_reaction_queue.h"

extern u16 data_0229da64;

void * func_0208109c(void)
{
    data_0229da68.writeIndex = 0;
    data_0229da68.readIndex = data_0229da68.writeIndex;
    data_0229da64 = 0;
    return &data_0229da64;
}
