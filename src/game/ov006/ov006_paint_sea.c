#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"

int Ov006_IsNotSeaTile(int x,
                       int y); /* TRUE unless the tile is sea, shoal or reef */
int Ov006_IsBridgeTile(int x, int y); /* checks bridge tiles only */
int Ov006_GetTilePropertyKind(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_BreakCellBlock(int x, int y);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_TurnToSea(int x, int y);
void Ov006_RemoveUnitOnImpassableTile(int x, int y);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define CELL_SEA 42 /* the cell Ov006_FillMapForScreen uses for sea */

/* Paints sea on tile x, y in the editor. A land tile (some bridges excepted)
 * first clears what Ov006_GetTilePropertyKind finds there and breaks any cell block
 * it was part of. Sea, shoal and reef stay; sea only gets its cell reset to
 * the plain sea cell. Last, Ov006_RemoveUnitOnImpassableTile refits the tile. */
void Ov006_PaintSea(int x, int y) {
  if (Ov006_IsNotSeaTile(x, y)) {
    if (Ov006_IsBridgeTile(x, y)) {
      return;
    }
    if (Ov006_GetTilePropertyKind(x, y)) {
      Ov006_RemoveProperty(x, y);
    }
    data_ov006_0236d640->oldCell = STATE->mapTiles->mapCells[TILE_INDEX(x, y)];
    Ov006_BreakCellBlock(x, y);
    Ov006_StoreMapCell(x, y, CELL_SEA);
    Ov006_SetTerrain(x, y, TERRAIN_SEA);
    Ov006_RefitCellsAround(x, y);
  } else {
    switch (STATE->mapTiles->tiles[TILE_INDEX(x, y)]) {
      case TERRAIN_SEA:
        Ov006_TurnToSea(x, y);
        if (STATE->mapTiles->mapCells[TILE_INDEX(x, y)] != CELL_SEA) {
          Ov006_StoreMapCell(x, y, CELL_SEA);
          Ov006_SetTerrain(x, y, TERRAIN_SEA);
        }
        break;
      case TERRAIN_SHOAL:
      case TERRAIN_REEF:
        break;
    }
  }
  Ov006_RemoveUnitOnImpassableTile(x, y);
}
