#include "types.h"
#include "game/map_select.h"
#include "game/Unk_02088d04.h"
#include "game/war_room_records.h"

void Gfx_RequestLayerCopy(int a);
void Bg_ClearRect(void *target, int x, int y, int w, int h);
void Bg_PutTileQuad(int x, int y, int tile, int size);
int MapSelect_GetRecordMarker(int list, int mapId);

extern Unk_027c0280 *data_027c0280;

/* As in MapSelect_DrawMapNames. */
#define MAP_ID (listBase->setup.lists[0].mapIds[index])
#define START_Y (STATE->gameMode == GAME_MODE_WAR_ROOM ? 9 : (STATE->twoMaps == 0 ? 5 : 7))

/* Redraws only the War Room record markers next to the map names of one page
 * of map list `list`, at the places MapSelect_DrawMapNames uses: clears the
 * marker column, then draws a marker for each map with a first-place record
 * in the current record table (TABLES.subStrip). */
void MapSelect_DrawRecordMarkers(int list, int page)
{
    int i;
    int x;
    int y;
    int index;
    MenuState *listBase;

    x = STATE->gameMode == GAME_MODE_WAR_ROOM ? 17 : 1;
    y = START_Y;
    /* Needed to match: the original narrows the arguments to u16. */
    Bg_ClearRect(data_027c0280->layer0Map, (u16)(x + 9), (u16)y, 2, (u16)(COLS * 2));
    listBase = (MenuState *)((u8 *)&gMenuState + list * sizeof(MapList));
    for (i = 0; i < ROWS * COLS; i++) {
        index = page * (ROWS * COLS) + i;
        if (index >= listBase->setup.lists[0].mapCount) {
            break;
        }
        if (gWarRoomRecords[TABLES.subStrip][MAP_ID - WAR_ROOM_FIRST_MAP][0].days != 0) {
            Bg_PutTileQuad(x + 9, y,
                          0x400 - ROWS * COLS * 32 - ((4 - MapSelect_GetRecordMarker(TABLES.subStrip, MAP_ID)) << 2), 1);
        }
        y += 2;
        if (y == START_Y + COLS * 2) {
            x += STATE->gameMode == GAME_MODE_WAR_ROOM ? 0 : 9;
            y = START_Y;
        }
    }
    Gfx_RequestLayerCopy(0);
}
