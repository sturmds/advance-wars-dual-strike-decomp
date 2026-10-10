#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int func_ov006_0235db00(int x, int y);
void func_ov006_02351828(int x, int y, int value);
void Ov006_SetTerrain(int x, int y, int terrain);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Stores the cell func_ov006_0235db00 picks for tile x, y: 42 (sea) when it
 * returns 0, or when it returns a negative value on a sea tile. A cell of 42
 * also sets the terrain to sea. */
void func_ov006_0235e04c(int x, int y)
{
    int cell = func_ov006_0235db00(x, y);

    if (cell == 0) {
        cell = 42;
    } else if (cell < 0) {
        if (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] == TERRAIN_SEA) {
            cell = 42;
        }
    }
    func_ov006_02351828(x, y, cell);
    if (cell == 42) {
        Ov006_SetTerrain(x, y, TERRAIN_SEA);
    }
}
