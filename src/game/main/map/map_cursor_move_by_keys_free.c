#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s16 data_02169454[16][2];   /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[16][2];   /* the same table from its y steps */

void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Like MapCursor_MoveByKeys without the range check: moves the map cursor
 * one tile by the d-pad keys in gKeypad->repeat, kept on the map, with
 * sound 0x10000 when it moves. */
void MapCursor_MoveByKeysFree(void)
{
    int dir = (gKeypad->repeat >> 4) & 15;
    int se = 0;
    int newX = GAME->cursorX + data_02169454[dir][0];
    int newY;

    if (newX >= 0 && newX < GAME->unk_1c4->mapWidth) {
        GAME->cursorX = newX;
        if (data_02169454[dir][0] != 0) {
            se = 0x10000;
        }
    }
    newY = GAME->cursorY + data_02169456[dir][0];
    if (newY >= 0 && newY < GAME->unk_1c4->mapHeight) {
        GAME->cursorY = newY;
        if (data_02169456[dir][0] != 0) {
            se = 0x10000;
        }
    }
    if (se != 0) {
        Sound_PlaySe(se);
    }
}
