#include "types.h"
#include "game/UnitDropTask.h"
#include "game/player.h"
#include "game/map_unit.h"
#include "game/Unk_02080440.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

MapUnit *Unit_Spawn(s16 x, s16 y, s16 type);
void PowerEffect_Start(int x, int y, int kind);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Script step of gUnitDropScript (Sensei's powers), after
 * UnitDrop_FindNextCity: spawns a unit of task->unitType with 9 HP at
 * (task->x, task->y), plays the power effect there when the tile is
 * shown, and moves on to the next tile. */
void UnitDrop_SpawnUnit(UnitDropTask *task)
{
    MapUnit *unit = Unit_Spawn(task->x, task->y, task->unitType);

    unit->hp = 90;
    if (STATE->mapTiles->unk_2338[STATE->mapTiles->rowStart[task->y] + task->x] != 0) {
        PowerEffect_Start((u8)task->x, (u8)task->y,
                      STATE->players[STATE->currentPlayer].front.unk_12);
    }
    task->x++;
}
