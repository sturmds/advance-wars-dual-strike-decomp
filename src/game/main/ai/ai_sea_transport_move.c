#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020b3c84(void);
void func_020b46c4(void);

/* Moves the acting sea transport: one routine when it carries a unit,
 * another when it is empty. */
void AiSeaTransport_Move(void)
{
    if (GAME->actingUnit->cargo[0] != 0) {
        func_020b46c4();
    } else {
        func_020b3c84();
    }
}
