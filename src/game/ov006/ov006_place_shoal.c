#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int func_ov006_02363d80(int x, int y);
void func_ov006_0235ddac(int x, int y);
void func_ov006_02351714(int x, int y, int terrain);
int Ov006_GetShoalGraphic(int x, int y);
void func_ov006_02351828(int x, int y, int value);
int func_ov006_02351780(int x, int y, int terrain);
void func_ov006_02363238(int x, int y);

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Tries to put shoal at (x, y). 0: refused (func_ov006_02363d80 or
 * Ov006_GetShoalGraphic is negative, and the old tile is put back in the second
 * case); 1: placed. */
int Ov006_PlaceShoal(int x, int y)
{
    u8 oldTile;
    int result;

    if (func_ov006_02363d80(x, y) < 0) {
        return 0;
    }
    func_ov006_0235ddac(x, y);
    oldTile = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];
    func_ov006_02351714(x, y, TERRAIN_SHOAL);
    result = Ov006_GetShoalGraphic(x, y);
    if (result < 0) {
        func_ov006_02351714(x, y, oldTile);
        return 0;
    }
    func_ov006_02351828(x, y, result);
    if (func_ov006_02351780(x, y, TERRAIN_SEA)) {
        return 1;
    }
    func_ov006_02363238(x, y);
    return 1;
}
