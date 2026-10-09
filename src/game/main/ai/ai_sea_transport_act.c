#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_0209c948(void);
void func_020a9854(void);
void func_020af540(void);
void func_020af5f8(void);
void func_020b2904(void);
void func_020b320c(void);

/* Queue routine for Black Boats and Landers, set by
 * AiAction_QueueSeaTransports. An empty Lander takes func_020af5f8, the
 * step foot units also run; a loaded Lander or a Black Boat takes
 * func_020af540. */
void AiSeaTransport_Act(void)
{
    func_0209c948();
    if (GAME->actingUnit->type == UNIT_LANDER && GAME->actingUnit->cargo[0] == 0) {
        func_020af5f8();
    } else {
        func_020af540();
    }
    func_020b320c();
    func_020a9854();
    func_020b2904();
}
