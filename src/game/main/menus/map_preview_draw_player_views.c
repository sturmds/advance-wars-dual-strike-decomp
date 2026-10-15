#include "types.h"
#include "game/map_data.h"
#include "game/map_header.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

void Map_LoadData(u32 mapId);
void Map_LoadArmyColours(void);
void Map_LoadArmyColoursFromPlayers(void);
int Campaign_IsHardMode(void);
u8 func_020c3d08(u32 mapId);
void func_020c3e5c(void);
void Player_SaveArmyColours(void);
void SavedArmyColours_Reset(void);
void MapPreview_DrawBlock(void* dst, s16 x, s16 y, int width, int height);
void MapPreview_DrawBlockUnits(void* dst, u16 x, u16 y, int width, int height);

#define MAP_ID (MAP_STATE->unk_1c4->mapId)

#define VIEW_WIDTH 16
#define VIEW_HEIGHT 8

/* Draws one view of the loaded map for each player of the map into dst: a
 * 16 by 8 tile picture, then a 16 by 8 picture of the units on it, in
 * blocks of 0x800 bytes. Each view is centred on the player's HQ and kept
 * inside the map; a few campaign maps name another centre. The terrain takes
 * its army colours from the map header (game mode 3) or from the players
 * (any other mode), the units from the players too, except in game modes 1
 * and 2, where they get none. */
void MapPreview_DrawPlayerViews(u8 (*dst)[32]) {
  int y;
  int mapX;
  int mapY;
  int view;
  int player;
  int x;
  u8 hq[ARMY_SLOT_COUNT][2];

  Map_LoadData(MAP_ID);
  if (MAP_STATE->gameMode == 3) {
    Map_LoadArmyColours();
  } else {
    Map_LoadArmyColoursFromPlayers();
  }

  for (mapY = 0; mapY < data_0229eadc->height; mapY++) {
    for (mapX = 0; mapX < data_0229eadc->width; mapX++) {
      u8 tile =
          MAP_STATE->tileCodeTerrain
              [data_0229eadc->tileCodes[mapY * data_0229eadc->width + mapX]];

      if ((tile & 0xe0) != 0) {
        if (MAP_TILE_TERRAIN(tile) == TERRAIN_HQ) {
          hq[MAP_TILE_OWNER(tile)][0] = mapX;
          hq[MAP_TILE_OWNER(tile)][1] = mapY;
        }
      }
    }
  }

  if ((u8)(MAP_STATE->gameMode + 255) <= 1) {
    SavedArmyColours_Reset();
  } else {
    Player_SaveArmyColours();
  }

  for (view = 0; view < func_020c3d08(MAP_ID); view++) {
    player = view + 1;
    if (player == 1 && MAP_ID == MAP_JAKES_TRIAL &&
        Campaign_IsHardMode() == 0) {
      x = 6;
      y = 7;
    } else if (player == 2 && MAP_ID == MAP_VERDANT_HILLS) {
      x = 8;
      y = 18;
    } else if (player == 2 && MAP_ID == MAP_MUCK_AMOK) {
      x = 7;
      y = 9;
    } else if (player == 2 && MAP_ID == MAP_DARK_AMBITION) {
      x = 1;
      y = 1;
    } else if (player == 3 && MAP_ID == MAP_DARK_AMBITION) {
      x = 1;
      y = 16;
    } else {
      x = hq[player][0];
      y = hq[player][1];
    }
    x -= VIEW_WIDTH / 2;
    y -= VIEW_HEIGHT / 2;
    if (x < 0) {
      x = 0;
    } else if (data_0229eadc->width - x < VIEW_WIDTH) {
      x = data_0229eadc->width - VIEW_WIDTH;
    }
    if (y < 0) {
      y = 0;
    } else if (data_0229eadc->height - y < VIEW_HEIGHT) {
      y = data_0229eadc->height - VIEW_HEIGHT;
    }
    MapPreview_DrawBlock(dst + view * 64, x, y, VIEW_WIDTH, VIEW_HEIGHT);
    MapPreview_DrawBlockUnits(dst + (view * 64 + 32), x, y, VIEW_WIDTH,
                              VIEW_HEIGHT);
  }
  func_020c3e5c();
}
