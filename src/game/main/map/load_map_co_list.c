#include "types.h"
#include "game/map_header.h"
#include "game/co_select_screen.h"

extern u8 gDefaultCoList[];

/* Copies the map's CO list (MapHeader.coList) into data_022a7a68, or all of
 * gDefaultCoList when the map has none. Campaign missions use the map list
 * to limit which COs the player can pick. */
void LoadMapCoList(int mapId) {
  int i;
  int count;
  const u8* list = gDefaultCoList;
  const u8* mapList = Ov000_gMapHeaders[mapId].coList;

  count = 0;
  if (mapList != NULL) {
    list = mapList;
  }
  i = 0;
  while (list[i] != 0) {
    data_022a7a68[count++] = list[i++];
  }
  data_022a7a68[count] = 0;
}
