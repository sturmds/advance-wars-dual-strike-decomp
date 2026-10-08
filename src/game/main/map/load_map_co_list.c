#include "types.h"
#include "game/map_header.h"

extern MapHeader Ov000_gMapHeaders[];
extern u8 gDefaultCoList[];
extern u8 data_022a7a68[];

/* Copies the map's CO list (header +0x20) into data_022a7a68, or all of
 * gDefaultCoList when the map has none. Campaign missions use the map list
 * to limit which COs the player can pick. */
void LoadMapCoList(int mapId)
{
    int i;
    int count;
    u8 *list = gDefaultCoList;
    u8 *mapList = (u8 *)Ov000_gMapHeaders[mapId].unk_20;

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
