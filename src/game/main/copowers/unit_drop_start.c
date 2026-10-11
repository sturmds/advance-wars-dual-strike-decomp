#include "types.h"
#include "game/UnitDropTask.h"
#include "game/Unk_02080440.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020e1be0(int player);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* First step of gUnitDropScript (Sensei's powers): starts the search for
 * drop sites at tile (0, 0) and starts the highlight of the current
 * player's power (func_020e1be0 reads highlightEffect and highlightColor
 * of the front CO's block). */
void UnitDrop_Start(UnitDropTask *task)
{
    task->x = 0;
    task->y = 0;
    func_020e1be0(STATE->currentPlayer);
}
