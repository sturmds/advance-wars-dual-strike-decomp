#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

void MovePath_AddTile(s8 x, s8 y);

extern MovePath *data_02166e7c;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define LAST_X (data_02166e7c->x[data_02166e7c->lastTile])
#define LAST_Y (data_02166e7c->y[data_02166e7c->lastTile])

/* Walks the steps in unk_170 (PATH_STEP_*, ending at PATH_STEP_END or -1)
 * and appends one tile to the move path for each move step. */
void MovePath_AppendSteps(void)
{
    s8 i = 0;

    for (;;) {
        switch ((s8)GAME->unk_170[i++]) {
        case -1:
        case PATH_STEP_END:
            return;
        case PATH_STEP_LEFT:
            MovePath_AddTile(LAST_X - 1, LAST_Y);
            break;
        case PATH_STEP_RIGHT:
            MovePath_AddTile(LAST_X + 1, LAST_Y);
            break;
        case PATH_STEP_UP:
            MovePath_AddTile(LAST_X, LAST_Y - 1);
            break;
        case PATH_STEP_DOWN:
            MovePath_AddTile(LAST_X, LAST_Y + 1);
            break;
        case 9: /* needed to match: the jump table runs up to 9; in UnitPath_CountSteps 9 takes a parameter byte */
            break;
        }
    }
}
