#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void MovePath_Truncate(int count);
void MovePath_FillRangeFromEnd(void);
void func_01ffa02c(int x, int y, u8 *out);
void MovePath_AppendSteps(void);

/* Cuts the move path back to its first tile, then finds the steps to tile
 * (unk_28, unk_2a) into unk_170 and appends them. */
void MovePath_RebuildToTarget(void)
{
    MovePath_Truncate(1);
    MovePath_FillRangeFromEnd();
    func_01ffa02c(GAME->unk_28, GAME->unk_2a, GAME->unk_170);
    MovePath_AppendSteps();
}
