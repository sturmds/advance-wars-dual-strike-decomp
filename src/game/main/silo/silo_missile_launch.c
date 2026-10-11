#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/terrain.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void SiloMissile_LoadGfx(int tile, int palette);
void SiloMissileLaunch_CreateTask(int x, int y, int tile, int palette, Task *parent);
void MapSiteList_RefreshTiles(void);
void func_020c4018(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Map cell codes of a silo before and after its launch. */
#define MAP_CELL_SILO      384
#define MAP_CELL_SILO_USED 416

/* Sprite tile and palette of the missile. */
#define MISSILE_TILE    404
#define MISSILE_PALETTE 5

/* Fires the missile silo at tile (x, y): loads the missile graphics,
 * starts the launch task under `parent`, and turns a loaded silo into a
 * used one before the site list is rebuilt. */
void SiloMissile_Launch(int x, int y, Task *parent)
{
    u16 cell = STATE->mapTiles->mapCells[TILE_INDEX(x, y)];

    SiloMissile_LoadGfx(MISSILE_TILE, MISSILE_PALETTE);
    SiloMissileLaunch_CreateTask(x, y, MISSILE_TILE, MISSILE_PALETTE, parent);
    if (cell == MAP_CELL_SILO) {
        STATE->mapTiles->tiles[TILE_INDEX(x, y)] = TERRAIN_SILO_USED;
        STATE->mapTiles->mapCells[TILE_INDEX(x, y)] = MAP_CELL_SILO_USED;
    }
    MapSiteList_RefreshTiles();
    func_020c4018();
}
