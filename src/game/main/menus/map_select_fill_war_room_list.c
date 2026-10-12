#include "types.h"
#include "game/menu_state.h"
#include "game/map_header.h"

BOOL MapSelect_IsMapPickable(int mapId);
u8 Map_GetPlayerCount(u16 mapId);
int MapSelect_ListKindToMapCase(int kind);

#define KIND_WAR_ROOM_VARIANTS 12

/* Fills a War Room list: list kinds 9-11 hold the pickable maps of the
 * War Room map case with 2, 3 and 4 players, kind 12 the ones that have a
 * variant map id. Returns TRUE when the list has maps. */
BOOL MapSelect_FillWarRoomList(MapList* list, int kind) {
  int playerCount;
  int count = 0;
  int mapId;
  MapHeader* header;

  switch (kind) {
    case 9:
      playerCount = 2;
      break;
    case 10:
      playerCount = 3;
      break;
    case 11:
      playerCount = 4;
      break;
    default:
      playerCount = count;
      break;
  }
  list->unk_64 = kind;
  kind = MapSelect_ListKindToMapCase(6);
  header = Ov000_gMapHeaders;
  for (mapId = 0; mapId < MAP_COUNT; mapId++, header++) {
    if (kind != header->mapCase) {
      continue;
    }
    if (list->unk_64 == KIND_WAR_ROOM_VARIANTS) {
      if (header->variantMapIds[0] == 0) {
        continue;
      }
    } else if (header->variantMapIds[0] != 0 ||
               !MapSelect_IsMapPickable(mapId) ||
               playerCount != Map_GetPlayerCount(mapId)) {
      continue;
    }
    list->mapIds[count++] = mapId;
  }
  if (count == 0) {
    return FALSE;
  }
  list->mapCount = count;
  return TRUE;
}
