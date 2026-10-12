#include "types.h"
#include "game/menu_state.h"
#include "game/map_header.h"

BOOL MapSelect_IsMapPickable(int mapId);
u8 Map_GetPlayerCount(u16 mapId);
int MapSelect_ListKindToMapCase(int kind);
u8 func_02101964(u32 id);

/* Fills the list of one kind with the pickable maps of its map case. Nothing
 * is listed while the unlock flag of that case (func_02101964) is clear.
 * Case 7 also takes case 8. Returns 1 when the list has maps. */
int MapSelect_FillList(MapList *list, int kind, int playerCount)
{
    int count = 0;
    int mapId;
    MapHeader *header;

    list->unk_64 = kind;
    kind = MapSelect_ListKindToMapCase(kind);
    if (!func_02101964(kind)) {
        return 0;
    }
    header = Ov000_gMapHeaders;
    for (mapId = 0; mapId < MAP_COUNT; mapId++, header++) {
        if (kind == MAP_CASE_WAR_ROOM) {
            if (header->mapCase != MAP_CASE_WAR_ROOM && header->mapCase != MAP_CASE_WAR_ROOM_DS) {
                continue;
            }
        } else if (kind != header->mapCase) {
            continue;
        }
        if (!MapSelect_IsMapPickable(mapId)) {
            continue;
        }
        if (playerCount != 0 && playerCount != Map_GetPlayerCount(mapId)) {
            continue;
        }
        list->mapIds[count++] = mapId;
    }
    if (count == 0) {
        return 0;
    }
    list->mapCount = count;
    return 1;
}
