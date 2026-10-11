#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s16 data_02169454[16][2];   /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[16][2];   /* the same table from its y steps */

void func_020c3b1c(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* For the d-pad keys held in gKeypad->unk_00: puts the cursor one tile in
 * from the edge of the visible area (16x12 tiles, or the map if smaller) on
 * each side it is pushed towards, runs func_020c3b1c, then steps it one
 * tile that way, kept on the map. The declaration order is needed to match. */
void MapCursor_StepPastViewEdge(void)
{
    int dir;
    Unk_02080440_1c4 *view;
    int newX;
    int width;
    int height;
    Unk_02080440_48 *game;
    int newY;

    game = GAME;
    /* needed to match: this read adds the scaled index first */
    view = STATE_IDX_FIRST->unk_1c4;
    dir = (gKeypad->unk_00 >> 4) & 15;
    width = 16;
    height = 12;

    if (view->mapWidth < width) {
        width = view->mapWidth;
    }
    if (view->mapHeight < height) {
        height = view->mapHeight;
    }
    if (data_02169454[dir][0] < 0) {
        game->unk_28 = (view->scrollX >> 4) + 1;
    }
    if (data_02169454[dir][0] > 0) {
        GAME->unk_28 = width - 2 + (GAME->unk_1c4->scrollX >> 4);
    }
    if (data_02169456[dir][0] < 0) {
        GAME->unk_2a = (GAME->unk_1c4->scrollY >> 4) + 1;
    }
    if (data_02169456[dir][0] > 0) {
        GAME->unk_2a = height - 2 + (GAME->unk_1c4->scrollY >> 4);
    }
    func_020c3b1c();
    newX = GAME->unk_28 + data_02169454[dir][0];
    if (newX >= 0 && newX < GAME->unk_1c4->mapWidth) {
        GAME->unk_28 = newX;
    }
    newY = GAME->unk_2a + data_02169456[dir][0];
    if (newY >= 0 && newY < GAME->unk_1c4->mapHeight) {
        GAME->unk_2a = newY;
    }
}
