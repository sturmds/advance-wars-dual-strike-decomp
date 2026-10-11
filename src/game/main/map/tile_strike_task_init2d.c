#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02022404.h"
#include "game/tile_strike_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_02148eec[];

Unk_02022404 *func_02022478(void *file, int a, int b);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Makes the falling sprite, above the top edge of the view. */
void TileStrikeTask_Init2d(TileStrikeTask *task)
{
    task->sprite = func_02022478(data_02148eec, 1, 2);
    task->x = task->tileX * 16 + 8;
    task->y = GAME->unk_1c4->scrollY + 16;
    task->sprite->unk_24 = 0x5194;
}
