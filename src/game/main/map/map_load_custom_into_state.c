#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/camera_pan.h"

void DesignMap_LoadIntoState(int index, Unk_02080440_1c4* map);
void Camera_ResetToPreset0(void* a);

extern Camera data_022a7d5c;

/* Map_LoadIntoState for a custom map id: DesignMap_LoadIntoState loads
 * slot mapId - MAP_CUSTOM_FIRST into the state's map data, then each tile
 * code in mapCells is converted through tileCodeTerrain into tiles. */
void Map_LoadCustomIntoState(int mapId) {
  int x;
  int y;

  DesignMap_LoadIntoState((u8)(mapId - MAP_CUSTOM_FIRST), MAP_STATE->unk_1c4);
  Camera_ResetToPreset0(&data_022a7d5c);
  MAP_STATE->unk_1c4->scrollX = 0;
  MAP_STATE->unk_1c4->scrollY = 0;
  MAP_STATE->unk_1c4->unk_22 = 0;
  MAP_STATE->unk_1c4->unk_24 = 0;
  MAP_STATE->unk_1c4->unk_26 = 0;
  MAP_STATE->unk_1c4->unk_28 = 0;
  MAP_STATE->unk_1c4->scrollFlags = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] =
          MAP_STATE
              ->tileCodeTerrain[MAP_STATE->mapTiles->mapCells
                                    [x + MAP_STATE->mapTiles->rowStart[y]]];
    }
  }
}
