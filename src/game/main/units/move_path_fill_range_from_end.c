#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

extern MovePath *data_02166e7c;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_01ffa488(int player, int x, int y, int unitType, int movesLeft, int flag);

/* Calls func_01ffa488 (guess: fills the movement range) for the acting
 * unit from the last tile of the move path, with the move points left
 * there. */
void MovePath_FillRangeFromEnd(void)
{
    func_01ffa488((u16)((GAME->actingUnitIndex >> 6) + 1),
                  data_02166e7c->x[data_02166e7c->lastTile],
                  data_02166e7c->y[data_02166e7c->lastTile],
                  GAME->actingUnit->type,
                  data_02166e7c->movesLeft[data_02166e7c->lastTile], 1);
}
