#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_GetPipeLink(int x, int y, int dir);   /* guess: how tile x, y joins its neighbour in direction dir (0: not at all) */

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;
/* Steps per direction: left, right, up, down. Copies of Ov006_gNeighborDx
 * and Ov006_gNeighborDy that only this function reads. */
extern s16 data_ov006_0236d608[4];
extern s16 data_ov006_0236d5f8[4];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

#define DIR_LEFT  0
#define DIR_RIGHT 1
#define DIR_UP    2
#define DIR_DOWN  3

#define JOIN_KIND_2 2   /* a Ov006_GetPipeLink result; meaning unknown */

/* Looks at the neighbour of x, y in direction dir. Returns JOIN_KIND_2 when
 * the neighbour is off the map. When the neighbour holds cell 0x162 or
 * 0x163, returns JOIN_KIND_2 if the cell runs across dir (0x163 for left
 * and right, 0x162 for up and down) and one of its two cross sides joins
 * with kind 2; else 0. Otherwise, if x, y does not join the neighbour with
 * kind 2 or 3, returns JOIN_KIND_2; if it does, returns how many of the
 * neighbour's other sides join with kind 2. */
int func_ov006_02369bc4(int x, int y, int dir)
{
    int join = Ov006_GetPipeLink(x, y, dir);
    int count = 0;
    int nx = x + data_ov006_0236d608[dir];
    int ny;
    u16 cell;
    int i;

    if (nx < 0 || nx >= STATE->unk_1c4->mapWidth
        || (ny = y + data_ov006_0236d5f8[dir]) < 0 || ny >= STATE->unk_1c4->mapHeight) {
        return JOIN_KIND_2;
    }
    cell = CELL_AT(nx, ny);
    if (cell == 0x162 || cell == 0x163) {
        if ((u32)dir <= DIR_RIGHT) {
            if (cell == 0x163) {
                if (Ov006_GetPipeLink(nx, ny, DIR_UP) == JOIN_KIND_2
                    || Ov006_GetPipeLink(x + data_ov006_0236d608[dir], y + data_ov006_0236d5f8[dir], DIR_DOWN) == JOIN_KIND_2) {
                    return JOIN_KIND_2;
                }
                return count;
            }
            return count;
        }
        if ((u32)(dir - DIR_UP) <= DIR_DOWN - DIR_UP) {
            if (cell == 0x162) {
                if (Ov006_GetPipeLink(nx, ny, DIR_LEFT) == JOIN_KIND_2
                    || Ov006_GetPipeLink(x + data_ov006_0236d608[dir], y + data_ov006_0236d5f8[dir], DIR_RIGHT) == JOIN_KIND_2) {
                    return JOIN_KIND_2;
                }
                return count;
            }
            return count;
        }
    }
    if (join != 3 && join != JOIN_KIND_2) {
        return JOIN_KIND_2;
    }
    for (i = 0; i < 4; i++) {
        /* Skips the side that points back at x, y. The original tests the x
         * steps twice, so for up and down it skips both up and down. */
        if (data_ov006_0236d608[dir] + data_ov006_0236d608[i] != 0
            || data_ov006_0236d608[dir] + data_ov006_0236d608[i] != 0) {
            if (Ov006_GetPipeLink(x + data_ov006_0236d608[dir], y + data_ov006_0236d5f8[dir], i) == JOIN_KIND_2) {
                count++;
            }
        }
    }
    return count;
}
