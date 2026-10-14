#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/camera_pan.h"

void DesignMap_LoadIntoState(int index, Unk_02080440_1c4* map);
void Camera_ResetToPreset0(void* a);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Camera data_022a7d5c;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Map_LoadIntoState for a custom map id: DesignMap_LoadIntoState loads
 * slot mapId - MAP_CUSTOM_FIRST into the state's map data, then each tile
 * code in mapCells is converted through tileCodeTerrain into tiles. */
void Map_LoadCustomIntoState(int mapId) {
  int x;
  int y;

  DesignMap_LoadIntoState((u8)(mapId - MAP_CUSTOM_FIRST), STATE->unk_1c4);
  Camera_ResetToPreset0(&data_022a7d5c);
  STATE->unk_1c4->scrollX = 0;
  STATE->unk_1c4->scrollY = 0;
  STATE->unk_1c4->unk_22 = 0;
  STATE->unk_1c4->unk_24 = 0;
  STATE->unk_1c4->unk_26 = 0;
  STATE->unk_1c4->unk_28 = 0;
  STATE->unk_1c4->scrollFlags = 0;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] =
          STATE->tileCodeTerrain
              [STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]]];
    }
  }
}
