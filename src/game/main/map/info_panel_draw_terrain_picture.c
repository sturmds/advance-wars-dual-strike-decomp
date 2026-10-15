#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/info_panel.h"
#include "game/terrain.h"

extern s16 data_0216635c[INFO_PANEL_SIDES][3]; /* terrain box x */
extern u8 data_02166348[]; /* sprite shape of the terrain icon */
extern u8 data_02166330[]; /* sprite shape of the terrain picture */

int Map_GetDisplayTile(int x, int y);
u8 Map_GetTileColour(int tile);
u8 Map_GetTileArmy(int tile);
u8* Terrain_GetIconTiles(int terrain);
u8* Palette_GetBlock(int index, int player);
const void* Terrain_GetPalette(int id, int army);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

#define GFX data_027c0280

/* Object tiles the two sprites use; palettes 1 and 6 (Palette_Load at 0x2c0). */
#define ICON_TILE 0x338
#define PICTURE_TILE 0x354

/* Terrain box of the info panel: the icon of the tile's terrain and its
 * picture, which Palette_GetBlock gives per terrain (the HQ per owner). An
 * owned property is shown in its owner's colour only where unk_2338 is set
 * for the tile; an HQ always is. */
void InfoPanel_DrawTerrainPicture(int tileX, int tileY, int panelX, int panelY,
                                  int side) {
  int tile = Map_GetDisplayTile(tileX, tileY);
  int terrain = MAP_TILE_TERRAIN(tile);
  int colour = Map_GetTileColour(tile);
  int owner;
  s16* boxX;

  Map_GetTileArmy(tile); /* result unused */
  owner = MAP_TILE_OWNER(tile);
  TransferQueue_Add(Terrain_GetIconTiles(terrain),
                    GFX->objVram + ICON_TILE * 32, 0x100);
  boxX = &data_0216635c[side][0];
  Oam_PutSprite((data_021663f4.unk_12[1].x + (panelX + *boxX)) & 0x1ff,
                (panelY + data_021663f4.unk_12[1].y) | 0x400, data_02166348,
                (1 << 12) | ICON_TILE);
  if (MAP_STATE->mapTiles
              ->unk_2338[MAP_STATE->mapTiles->rowStart[tileY] + tileX] == 0 &&
      terrain != TERRAIN_HQ) {
    colour = 0;
  }
  TransferQueue_Add(Palette_GetBlock(terrain, owner),
                    GFX->objVram + PICTURE_TILE * 32, 0x100);
  Palette_Load(Terrain_GetPalette(terrain, colour), 0x2c0, 32);
  Oam_PutSprite((data_021663f4.unk_12[0].x + (panelX + *boxX)) & 0x1ff,
                ((panelY + data_021663f4.unk_12[0].y) & 0xff) | 0x400,
                data_02166330, (6 << 12) | PICTURE_TILE);
}
