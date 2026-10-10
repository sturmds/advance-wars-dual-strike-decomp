#include "types.h"
#include "game/terrain.h"

int func_ov006_02351780(int x, int y, int terrain);
void Ov006_SetTerrain(int x, int y, int terrain);
int func_ov006_02364a08(int x, int y);

/* A reef tile at (x, y) that func_ov006_02364a08 rejects becomes sea. */
void Ov006_ReefToSeaIfRejected(int x, int y)
{
    if (!func_ov006_02351780(x, y, TERRAIN_REEF)) {
        return;
    }
    if (func_ov006_02364a08(x, y)) {
        return;
    }
    Ov006_SetTerrain(x, y, TERRAIN_SEA);
}
