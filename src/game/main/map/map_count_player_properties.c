#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"
#include "game/map_header.h"
#include "game/terrain.h"

int IncreaseFunds(int playerIndex, int terrain);
void Map_ApplySlotFlags(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Counts each player's properties and income from the map tiles (Base, City,
 * Airport, Port, Com Tower, HQ), and stores the HQ position in hqX and hqY.
 * Afterwards human players get no income while gameMode is 3 and
 * data_022a7b80.mode is 1 or 4. */
void Map_CountPlayerProperties(void) {
  int i;
  int slot;
  u8 tile;
  Player* player;
  int x;
  int y;

  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    STATE->players[i].unk_08 = 0;
    STATE->players[i].unk_10[0] = 0;
    STATE->players[i].unk_10[1] = 0;
    STATE->players[i].unk_10[2] = 0;
    STATE->players[i].unk_10[3] = 0;
    STATE->players[i].comTowers = 0;
    STATE->players[i].hqX |= 0x80;
  }
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      tile = TILE_AT(x, y);
      if ((tile & 0xe0) != 0) {
        player = &STATE->players[MAP_TILE_OWNER(tile)];
        switch (MAP_TILE_TERRAIN(tile)) {
          case TERRAIN_HQ:
            player->unk_08 +=
                IncreaseFunds(MAP_TILE_OWNER(tile), MAP_TILE_TERRAIN(tile));
            player->hqX = x;
            player->hqY = y;
            break;
          case TERRAIN_BASE:
            player->unk_08 +=
                IncreaseFunds(MAP_TILE_OWNER(tile), MAP_TILE_TERRAIN(tile));
            player->unk_10[0]++;
            break;
          case TERRAIN_CITY:
            player->unk_08 +=
                IncreaseFunds(MAP_TILE_OWNER(tile), MAP_TILE_TERRAIN(tile));
            player->unk_10[1]++;
            break;
          case TERRAIN_COM_TOWER:
            player->unk_08 +=
                IncreaseFunds(MAP_TILE_OWNER(tile), MAP_TILE_TERRAIN(tile));
            player->comTowers++;
            break;
          case TERRAIN_AIRPORT:
            player->unk_08 +=
                IncreaseFunds(MAP_TILE_OWNER(tile), MAP_TILE_TERRAIN(tile));
            player->unk_10[2]++;
            break;
          case TERRAIN_PORT:
            /* Read from the map again, not from tile: needed to match. */
            player->unk_08 += IncreaseFunds(MAP_TILE_OWNER(TILE_AT(x, y)),
                                            MAP_TILE_TERRAIN(TILE_AT(x, y)));
            player->unk_10[3]++;
            break;
        }
      }
    }
  }
  Map_ApplySlotFlags();
  if (STATE->gameMode == 3 &&
      (data_022a7b80.mode == 1 || data_022a7b80.mode == 4)) {
    for (slot = 1; slot < ARMY_SLOT_COUNT; slot++) {
      if (STATE->players[slot].controller == PLAYER_CONTROLLER_HUMAN) {
        STATE->players[slot].unk_08 = 0;
      }
    }
  }
}
