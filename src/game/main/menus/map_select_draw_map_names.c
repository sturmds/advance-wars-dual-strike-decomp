#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/war_room_records.h"

void Gfx_RequestLayerCopy(int a);
const void* Map_GetName(u32 mapId);
u8 Map_IsFlagSet(u32 mapId);
int func_02101904(u32 mapId);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Text_PrintWithTile(s16 x, s16 y, u16* map, const void* text, int a4,
                        u16 palette, u16 tile);
void Bg_PutTileQuad(int x, int y, int tile, int size);
int MapSelect_GetRecordMarker(int a, int mapId);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern MenuState gMenuState;
extern MapListTables data_0229c768;
extern u8 data_02167c90[];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define COLS \
  (STATE->gameMode == GAME_MODE_WAR_ROOM ? 7 : (STATE->twoMaps == 0 ? 9 : 8))
#define ROWS (STATE->gameMode == GAME_MODE_WAR_ROOM ? 1 : 3)
/* Needed to match: listBase is gMenuState moved on by `list` MapLists, so the
 * field offsets stay in the load instructions. */
#define MAP_ID (listBase->setup.lists[0].mapIds[index])
#define START_Y \
  (STATE->gameMode == GAME_MODE_WAR_ROOM ? 9 : (STATE->twoMaps == 0 ? 5 : 7))

/* Prints the names of the maps on one page of map list `list` into the layer 0
 * tile map, in columns that count down from the top. A map that is locked
 * (flag not set, id below 0x101) shows a placeholder text. In game mode 1 a
 * marker follows the name of the maps that have a first-place record in
 * gWarRoomRecords. */
void MapSelect_DrawMapNames(int list, int page) {
  int i;
  int x;
  int y;
  int index;
  int palette;
  MenuState* listBase;
  u16* map;
  int tile;

  x = STATE->gameMode == GAME_MODE_WAR_ROOM ? 17 : 1;
  y = START_Y;
  Mem_Zero32(0, data_027c0280->layer0Map, 0x800);
  listBase = (MenuState*)((u8*)&gMenuState + list * sizeof(MapList));
  for (i = 0; i < ROWS * COLS; i++) {
    index = page * (ROWS * COLS) + i;
    if (index >= listBase->setup.lists[0].mapCount) {
      break;
    }
    palette = func_02101904(MAP_ID) != 0 ? 0 : 1;
    if (MAP_ID < 0x101 && !Map_IsFlagSet(MAP_ID)) {
      Text_PrintWithTile(x, y, data_027c0280->layer0Map, &data_02167c90, 0,
                         palette, i * 16 + (0x400 - ROWS * COLS * 32));
    } else {
      tile = i * 16 + (0x400 - ROWS * COLS * 32);
      map = data_027c0280->layer0Map;
      Text_PrintWithTile(x, y, map, Map_GetName(MAP_ID), 0, palette, tile);
    }
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
      if (gWarRoomRecords[data_0229c768.subStrip][MAP_ID - WAR_ROOM_FIRST_MAP]
                         [0]
                             .days != 0) {
        Bg_PutTileQuad(
            x + 9, y,
            0x400 - ROWS * COLS * 32 -
                ((4 - MapSelect_GetRecordMarker(data_0229c768.subStrip, MAP_ID))
                 << 2),
            1);
      }
    }
    y += 2;
    if (y == START_Y + COLS * 2) {
      x += STATE->gameMode == GAME_MODE_WAR_ROOM ? 0 : 9;
      y = START_Y;
    }
  }
  Gfx_RequestLayerCopy(0);
}
