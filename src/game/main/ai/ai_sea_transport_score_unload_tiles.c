#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/range_map.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL AiSeaTransport_FindShoreTile(s16* out);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110

/* The caller sets gAiUnloadTest (a test) and gAiUnloadScore (a score). For
 * each Shoal or Port tile the acting sea transport can reach, this fills
 * rangeMap with the first cargo unit's moves from that tile (in
 * MapTiles.rangeScratch) and runs the test. When it passes, every Shoal
 * and Port tile in the cargo's reach gets the score in
 * MapTiles.rangeScratch2 if higher, and AI_STEP_UNLOAD_SCORED is set. Those tiles
 * are then struck from the transport's own range map, so each shore group
 * is handled once. */
void AiSeaTransport_ScoreUnloadTiles(void) {
  /* declaration order needed to match (register allocation) */
  s16 shore[2];
  int cargoMove;
  int col;
  MapUnit* cargo;
  int x;
  int y;
  int row;
  int terrain;
  int score;

  cargo = &STATE->units[STATE->actingUnit->cargo[0]];
  if (cargo->type == 0) {
    return;
  }
  cargoMove = AiUnit_GetUnloadMovement(cargo);
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  TileMap_Fill(MAP->rangeScratch2, 0);
  for (;;) {
    if (!AiSeaTransport_FindShoreTile(shore)) {
      return;
    }
    RangeMap_SetBuffer(MAP->rangeScratch);
    RangeMap_FillMoves(shore[0], shore[1], cargo->type, cargoMove, 0);
    if (gAiUnloadTest() == TRUE) {
      score = gAiUnloadScore();
      for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
          if (STATE->rangeMap[y][x] < 0) {
            continue;
          }
          terrain = MAP_TILE_TERRAIN(MAP->tiles[TILE(x, y)]);
          if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
            if ((u8)MAP->rangeScratch2[TILE(x, y)] < score) {
              MAP->rangeScratch2[TILE(x, y)] = score;
              gAiTurnState->stepFlags |= AI_STEP_UNLOAD_SCORED;
            }
            MAP->rangeMapTiles[TILE(x, y)] = -1;
          }
        }
      }
    } else {
      for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
          if (STATE->rangeMap[row][col] < 0) {
            continue;
          }
          terrain = MAP_TILE_TERRAIN(MAP->tiles[TILE(col, row)]);
          if (terrain == TERRAIN_SHOAL || terrain == TERRAIN_PORT) {
            MAP->rangeMapTiles[TILE(col, row)] = -1;
          }
        }
      }
    }
    RangeMap_SetBuffer(MAP->rangeMapTiles);
  }
}
