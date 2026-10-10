#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Ov006_IsSpecialCell(int x, int y);
int func_ov006_02361114(int x, int y);
int func_ov006_023622bc(int x, int y);
int Ov006_MouthHasNoOutlet(int x, int y);
void func_ov006_02351714(int x, int y, int terrain);
void func_ov006_02351828(int x, int y, int cell);
void func_ov006_02361684(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define FIX_NEIGHBOR(nx, ny, scoreTooLow)                                 \
    if (Ov006_IsSpecialCell(nx, ny)) {                                   \
        if (func_ov006_02361114(nx, ny) scoreTooLow                      \
            || !func_ov006_023622bc(nx, ny) || !Ov006_MouthHasNoOutlet(nx, ny)) { \
            func_ov006_02351714(nx, ny, TERRAIN_SEA);                    \
            func_ov006_02351828(nx, ny, MAP_CELL_SEA);          \
            func_ov006_02361684(nx, ny);                                 \
        }                                                                \
    }

/* Checks the eight tiles around x, y. A neighbour with one of the cells
 * Ov006_IsSpecialCell accepts (codes 252, 253, 284, 285) that no longer
 * passes func_ov006_02361114, func_ov006_023622bc and Ov006_MouthHasNoOutlet
 * turns back into plain sea; func_ov006_02361684 then updates the tiles
 * around it. Corner tiles fail at a score of 0, side tiles only below 0. */
void Ov006_ResetNeighborsToSea(int x, int y)
{
    if (y > 0) {
        FIX_NEIGHBOR(x, y - 1, < 0);
        if (x > 0) {
            FIX_NEIGHBOR(x - 1, y - 1, <= 0);
        }
        if (x < STATE->unk_1c4->mapWidth - 1) {
            FIX_NEIGHBOR(x + 1, y - 1, <= 0);
        }
    }
    if (y < STATE->unk_1c4->mapHeight - 1) {
        FIX_NEIGHBOR(x, y + 1, < 0);
        if (x > 0) {
            FIX_NEIGHBOR(x - 1, y + 1, <= 0);
        }
        if (x < STATE->unk_1c4->mapWidth - 1) {
            FIX_NEIGHBOR(x + 1, y + 1, <= 0);
        }
    }
    if (x > 0) {
        FIX_NEIGHBOR(x - 1, y, < 0);
    }
    if (x < STATE->unk_1c4->mapWidth - 1) {
        FIX_NEIGHBOR(x + 1, y, < 0);
    }
}
