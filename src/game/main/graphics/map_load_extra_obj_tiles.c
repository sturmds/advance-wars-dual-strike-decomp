#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/map_object.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
BOOL func_020fa6f8(void);
const char* Tileset_GetPaletteFileName(void);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
MapObject* Map_FindObjectOfKind(int kind);

extern const char*
    data_ov000_022dbd34[];   /* MapHeader.extraObjTileFile of map 0 */
extern char data_021682f4[]; /* "0a7" */
extern char data_021682f8[]; /* "/bmap" */
extern char data_02168300[]; /* "0a8" */
extern char data_02168304[]; /* "0a9" */
extern char data_02168308[]; /* "0a4" */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* MapHeader.extraObjTileFile of a map, read through the symbol at its offset. */
#define HEADER_TILE_FILE(mapId) \
  (*(const char**)((u8*)data_ov000_022dbd34 + (mapId) * sizeof(MapHeader)))

/* Loads /bmap/0a8 into object VRAM at firstTile + 48, then one more /bmap
 * file at firstTile + 120: 0a4 (with the tileset palette at 0x380) on maps
 * with a volcano, else 0a9 on maps with a Black Obelisk, else 0a7 when
 * func_020fa6f8 returns 1, else the map header's file (extraObjTileFile: 0a5 or 0a6
 * on nine maps, NULL on the rest). The weather argument is not used. */
void Map_LoadExtraObjTiles(int weather, int firstTile) {
  const char* file = HEADER_TILE_FILE(MAP_STATE->unk_1c4->mapId);

  if (func_020fa6f8() == 1) {
    file = data_021682f4;
  }
  FS_ChangeDir(data_021682f8);
  File_UnpackTo(data_027c0280->objVram + (firstTile + 48) * 32, data_02168300);
  if (Map_FindObjectOfKind(MAP_OBJECT_BLACK_OBELISK) != NULL) {
    file = data_02168304;
  }
  if (Map_FindObjectOfKind(MAP_OBJECT_VOLCANO) != NULL) {
    file = data_02168308;
    Palette_LoadFromFile(Tileset_GetPaletteFileName(), 0, 0x380, 32);
  }
  if (file != NULL) {
    FS_ChangeDir(data_021682f8);
    File_UnpackTo(data_027c0280->objVram + (firstTile + 120) * 32, file);
  }
}
