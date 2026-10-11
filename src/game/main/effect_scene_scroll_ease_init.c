#include "types.h"
#include "game/Unk_021694c0.h"

void func_01ff8bc8(int layer, int scrollX, int scrollY);

/* Resets the layer 3 scroll and sets up the ease that the Step task runs. */
void EffectScene_ScrollEaseInit(Unk_021694c0Task *task)
{
    func_01ff8bc8(3, 0, 0);
    task->easeFrom = 0;
    task->easeTo = 64;
    task->easeElapsed = 0;
    task->easeDuration = 0x3c00;
}
