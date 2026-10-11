#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

u32 Rng_Next(void);
u32 MainLoop_GetFrameCount(void);
void Rng_SetState(u32 state);
void Ov006_SetMapCell(int x, int y, int cell);
void func_ov006_02359c20(void);
void Ov006_ScatterMountains(void);
void func_ov006_02359a38(TilePoint *hq1, TilePoint *hq2);
void func_ov006_02359744(TilePoint *hq1, TilePoint *hq2, TilePoint *city1, TilePoint *city2, TilePoint *middle);
void func_ov006_02359608(TilePoint *hq1, TilePoint *hq2, TilePoint *city1, TilePoint *city2, TilePoint *middle);
void Ov006_ScatterWoods(void);
void Ov006_AddShoals(void);
void Ov006_ScatterReefs(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_PLAIN 1

/* Builds a random 2-army map: reseeds the random generator from the frame
 * count, fills the map with plain, then adds sea, mountains, the two HQs,
 * cities around them, roads between them, woods, shoals and reefs. */
void Ov006_GenerateRandomMap(void)
{
    TilePoint hq1;
    TilePoint hq2;
    TilePoint city1;
    TilePoint city2;
    TilePoint middle;
    int x;
    int y;

    Rng_SetState(Rng_Next() + MainLoop_GetFrameCount());
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            Ov006_SetMapCell(x, y, CELL_PLAIN);
        }
    }
    func_ov006_02359c20();
    Ov006_ScatterMountains();
    func_ov006_02359a38(&hq1, &hq2);
    func_ov006_02359744(&hq1, &hq2, &city1, &city2, &middle);
    func_ov006_02359608(&hq1, &hq2, &city1, &city2, &middle);
    Ov006_ScatterWoods();
    Ov006_AddShoals();
    Ov006_ScatterReefs();
}
