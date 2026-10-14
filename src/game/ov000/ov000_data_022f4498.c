#include "types.h"
#include "game/terrain.h"

extern u8 data_ov000_022d3118[];
extern u8 data_ov000_022d3ad0[];
extern u8 data_ov000_022d4048[];
extern u8 data_ov000_022d4610[];
extern u8 data_ov000_022d4f8c[];
extern u8 data_ov000_022d5508[];
extern u8 data_ov000_022d5a0c[];
extern u8 data_ov000_022d6f8c[];
extern u8 data_ov000_022d7534[];
extern u8 data_ov000_022d7b6c[];
extern u8 data_ov000_022d816c[];
extern const u32 data_ov000_022d87ac[];
extern const u32 data_ov000_022d36c0[];
extern const u32 data_ov000_022d4028[];
extern const u32 data_ov000_022d45f0[];
extern const u32 data_ov000_022d4b18[];
extern const u32 data_ov000_022d54e8[];
extern const u32 data_ov000_022d59ec[];
extern const u32 data_ov000_022d5fa8[];
extern const u32 data_ov000_022d7514[];
extern const u32 data_ov000_022d7b4c[];
extern const u32 data_ov000_022d814c[];
extern const u32 data_ov000_022d878c[];
extern const u32 data_ov000_022d8cf4[];
extern u8 data_ov000_022d36e0[];
extern const u32 data_ov000_022d4b38[];
extern const u32 data_ov000_022d3ab0[];
extern const u32 data_ov000_022d4f6c[];

u32 Ov000_gKoalCOP[] = {
#include "data/ov000/Ov000_gKoalCOP.inc.c"
};
u32 Ov000_gKoalD2D[] = {
#include "data/ov000/Ov000_gKoalD2D.inc.c"
};
u32 Ov000_gKindleD2D[] = {
#include "data/ov000/Ov000_gKindleD2D.inc.c"
};
u32 Ov000_gJakeSCOP[] = {
#include "data/ov000/Ov000_gJakeSCOP.inc.c"
};
u32 Ov000_gKoalSCOP[] = {
#include "data/ov000/Ov000_gKoalSCOP.inc.c"
};
u32 Ov000_gKindleSCOP[] = {
#include "data/ov000/Ov000_gKindleSCOP.inc.c"
};
u32 Ov000_gJakeD2D[] = {
#include "data/ov000/Ov000_gJakeD2D.inc.c"
};
u32 Ov000_gJakeCOP[] = {
#include "data/ov000/Ov000_gJakeCOP.inc.c"
};
u32 data_ov000_022f4698[] = {
#include "data/ov000/data_ov000_022f4698.inc.c"
};
u32 data_ov000_022f4678[] = {
#include "data/ov000/data_ov000_022f4678.inc.c"
};
u32 data_ov000_022f4658[] = {
#include "data/ov000/data_ov000_022f4658.inc.c"
};
u8 data_ov000_022f4638[] = {
#include "data/ov000/data_ov000_022f4638.inc.c"
};
u32 data_ov000_022f4618[] = {
#include "data/ov000/data_ov000_022f4618.inc.c"
};
u32 data_ov000_022f45f8[] = {
#include "data/ov000/data_ov000_022f45f8.inc.c"
};
s8 Ov000_gIncomeTerrain[] = {
#include "data/ov000/Ov000_gIncomeTerrain.inc.c"
};
s8 Ov000_gUrbanTerrain[] = {
#include "data/ov000/Ov000_gUrbanTerrain.inc.c"
};
s8 Ov000_gGroundDeployTerrain[] = {
#include "data/ov000/Ov000_gGroundDeployTerrain.inc.c"
};
s8 data_ov000_022f4578[] = {
#include "data/ov000/data_ov000_022f4578.inc.c"
};
u32 Ov000_gKindleCOP[] = {
#include "data/ov000/Ov000_gKindleCOP.inc.c"
};
u32 data_ov000_022f4538[] = {
#include "data/ov000/data_ov000_022f4538.inc.c"
};
s8 Ov000_gCostDefaultMovement[][TERRAIN_ID_COUNT] = {
#include "data/ov000/Ov000_gCostDefaultMovement.inc.c"
};
s8 Ov000_gCostPathfinderMovement[][TERRAIN_ID_COUNT] = {
#include "data/ov000/Ov000_gCostPathfinderMovement.inc.c"
};
s8 Ov000_gCostPrairiePathMovement[][TERRAIN_ID_COUNT] = {
#include "data/ov000/Ov000_gCostPrairiePathMovement.inc.c"
};
s8 Ov000_gCostPrairieDogMovement[][TERRAIN_ID_COUNT] = {
#include "data/ov000/Ov000_gCostPrairieDogMovement.inc.c"
};
s8 Ov000_gCostFreeMovement[][TERRAIN_ID_COUNT] = {
#include "data/ov000/Ov000_gCostFreeMovement.inc.c"
};
TerrainStats Ov000_gTerrainStats[] = {
#include "data/ov000/Ov000_gTerrainStats.inc.c"
};
