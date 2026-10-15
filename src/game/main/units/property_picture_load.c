#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/terrain.h"
#include "sdk/fs.h"

extern char data_02169180[]; /* "/bmap" */
extern char data_021691a8[]; /* "0b9" */
extern char data_021691ac[]; /* "0bb" */

u8 Map_GetTileArmy(int tile);
void File_ReadAt(void* dest, const char* path, u32 offset, u32 size);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          u16 paletteOffset, int size);

/* Bytes of one property picture in /bmap/0bb. */
#define PICTURE_SIZE 0x400

/* Loads the picture of the property on map tile `tile` into sprite tile
 * gfxTile, and its owner's army colours from /bmap/0b9 into sprite palette
 * `palette`. /bmap/0bb holds one HQ per army style (1-5), then a city, base,
 * airport, port and lab; other terrain gets the city. The callers draw the
 * capture of a property (CaptureTask_ShowCompletion waits for 20 capture points). */
void PropertyPicture_Load(int tile, int gfxTile, int palette) {
  int army = Map_GetTileArmy(tile);
  int colour = MAP_STATE->players[MAP_TILE_OWNER(tile)].armyColour;
  u32 offset;

  switch (MAP_TILE_TERRAIN(tile)) {
    case TERRAIN_HQ:
      offset = (army - 1) * PICTURE_SIZE;
      break;
    case TERRAIN_CITY:
    default:
      offset = 5 * PICTURE_SIZE;
      break;
    case TERRAIN_BASE:
      offset = 6 * PICTURE_SIZE;
      break;
    case TERRAIN_AIRPORT:
      offset = 7 * PICTURE_SIZE;
      break;
    case TERRAIN_PORT:
      offset = 8 * PICTURE_SIZE;
      break;
    case TERRAIN_LAB:
      offset = 9 * PICTURE_SIZE;
      break;
  }
  FS_ChangeDir(data_02169180);
  File_ReadAt(data_027c0280->objVram + gfxTile * 32, data_021691ac, offset,
              PICTURE_SIZE);
  Palette_LoadFromFile(data_021691a8, colour << 5, (palette + 16) << 5, 32);
}
