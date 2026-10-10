#include "types.h"
#include "game/terrain.h"

int func_ov006_02351288(int x, int y);
void func_ov006_02351714(int x, int y, int terrain);
void func_ov006_02363198(int x, int y);
void func_ov006_0236146c(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void func_ov006_0235d7dc(int x, int y);
void func_ov006_02364844(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

/* Puts a mountain at (x, y), going through plain first when
 * func_ov006_02351288 returns 0, then runs the follow-up fixes for the tile. */
void Ov006_PlaceMountain(int x, int y)
{
    if (!func_ov006_02351288(x, y)) {
        func_ov006_02351714(x, y, TERRAIN_PLAIN);
    }
    func_ov006_02351714(x, y, TERRAIN_MOUNTAIN);
    func_ov006_02363198(x, y);
    func_ov006_0236146c(x, y);
    Ov006_ResetNeighborsToSea(x, y);
    func_ov006_0235d7dc(x, y);
    func_ov006_02364844(x, y);
    Ov006_BreakCellBlock(x, y);
}
