#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Directions of the cone */
#define CONE_DOWN  0
#define CONE_UP    1
#define CONE_LEFT  2
#define CONE_RIGHT 3

/* Writes value into a cone of range map tiles: length lines from (x, y)
 * in direction dir, 1 tile wide at the tip and 2 wider on each line after,
 * cut at the map edges. The cannon map objects use it for their fire
 * range, with their variant as dir. */
void RangeMap_FillCone(int x, int y, int dir, int length, s8 value)
{
    int col;                           /* needed to match: col and row are separate variables */
    int colEnd;
    int row;
    int rowEnd;
    int step;
    int t;                             /* line being filled */
    int count;

    step = 1;

    switch (dir) {
    case CONE_UP:
        for (t = y; t > y - length; t--, step++) {
            if (t < 0) {
                return;
            }
            col = x - (step - 1);
            count = step * 2 - 1;
            if (col < 0) {
                count += col;
                col = 0;
            }
            colEnd = col + count;
            if (colEnd > STATE->unk_1c4->mapWidth) {
                colEnd = STATE->unk_1c4->mapWidth;
            }
            for (; col < colEnd; col++) {
                STATE->rangeMap[t][col] = value;
            }
        }
        break;
    case CONE_DOWN:
        for (t = y; t < y + length; t++, step++) {
            if (t >= STATE->unk_1c4->mapHeight) {
                return;
            }
            col = x - (step - 1);
            count = step * 2 - 1;
            if (col < 0) {
                count += col;
                col = 0;
            }
            colEnd = col + count;
            if (colEnd > STATE->unk_1c4->mapWidth) {
                colEnd = STATE->unk_1c4->mapWidth;
            }
            for (; col < colEnd; col++) {
                STATE->rangeMap[t][col] = value;
            }
        }
        break;
    case CONE_LEFT:
        for (t = x; t > x - length; t--, step++) {
            if (t < 0) {
                return;
            }
            row = y - (step - 1);
            count = step * 2 - 1;
            if (row < 0) {
                count += row;
                row = 0;
            }
            rowEnd = row + count;
            if (rowEnd > STATE->unk_1c4->mapHeight) {
                rowEnd = STATE->unk_1c4->mapHeight;
            }
            for (; row < rowEnd; row++) {
                STATE->rangeMap[row][t] = value;
            }
        }
        break;
    case CONE_RIGHT:
        for (t = x; t < x + length; t++, step++) {
            if (t >= STATE->unk_1c4->mapWidth) {
                return;
            }
            row = y - (step - 1);
            count = step * 2 - 1;
            if (row < 0) {
                count += row;
                row = 0;
            }
            rowEnd = row + count;
            if (rowEnd > STATE->unk_1c4->mapHeight) {
                rowEnd = STATE->unk_1c4->mapHeight;
            }
            for (; row < rowEnd; row++) {
                STATE->rangeMap[row][t] = value;
            }
        }
        break;
    }
}
