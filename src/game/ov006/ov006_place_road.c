#include "types.h"
#include "game/terrain.h"

int Ov006_TileMatches(int x, int y, int terrain);   /* guess: the tile is on the map and has that terrain */
void Ov006_PlaceBridgeByPattern(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
int func_ov006_023670ec(int x, int y);               /* guess: picks the cell code for a road at x, y */
void Ov006_StoreMapCell(int x, int y, int cell);    /* guess: sets the cell code of tile x, y */
int func_ov006_02351238(int x, int y);               /* reads the cell code at x, y */
void func_ov006_0236146c(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void func_ov006_0235d7dc(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

/* The road tool at (x, y). On a river or bridge tile it puts a bridge
 * instead (Ov006_PlaceBridgeByPattern). Otherwise it sets road, picks the
 * road's cell, writes the stored cell back, and runs the same follow-up
 * fixes as Ov006_PlaceMountain. */
void Ov006_PlaceRoad(int x, int y)
{
    if (Ov006_TileMatches(x, y, TERRAIN_RIVER) || Ov006_TileMatches(x, y, TERRAIN_BRIDGE)) {
        Ov006_PlaceBridgeByPattern(x, y);
        return;
    }
    Ov006_SetTerrain(x, y, TERRAIN_ROAD);
    Ov006_StoreMapCell(x, y, func_ov006_023670ec(x, y));
    Ov006_StoreMapCell(x, y, func_ov006_02351238(x, y));
    func_ov006_0236146c(x, y);
    Ov006_ResetNeighborsToSea(x, y);
    func_ov006_0235d7dc(x, y);
    Ov006_CheckReefsAround(x, y);
    Ov006_BreakCellBlock(x, y);
}
