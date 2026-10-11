#include "types.h"
#include "game/map_select.h"

void *Mem_Alloc(u32 size);
void Mem_Free(void *p);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void MapSelect_BuildWarRoomLists(MapList *lists);
void MapSelect_BuildLists(MapList *lists, int a);

/* Points the map select menu of the current mode (War Room, or Versus
 * with one or two maps) at mapId: the list that holds it, its page and
 * its place on the page. Other modes keep the menu as it is. */
void MapSelect_PointAtMap(int mapId)
{
    MapListSet *set;
    int tab;
    int i;
    int j;

    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        tab = 3;
    } else if (STATE->gameMode == GAME_MODE_VERSUS) {
        if (STATE->twoMaps == 0) {
            tab = 0;
        } else {
            tab = 1;
        }
    } else {
        return;
    }
    set = Mem_Alloc(sizeof(MapListSet));
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        MapSelect_BuildWarRoomLists(set->lists);
    } else {
        MapSelect_BuildLists(set->lists, 0);
    }
    TABLES.subStrip = STATE->unk_59;
    for (i = 0; i < set->count; i++) {
        /* Needed to match: the test inside the loop keeps it unrotated. */
        for (j = 0; ; j++) {
            if (j >= set->lists[i].mapCount) {
                break;
            }
            if (mapId == set->lists[i].mapIds[j]) {
                TABLES.listOfTab[tab] = i;
                TABLES.unk_0d0[tab][set->lists[i].unk_64] = HwDiv32(j, ROWS * COLS);
                TABLES.unk_000[tab][set->lists[i].unk_64][TABLES.unk_0d0[tab][set->lists[i].unk_64]] =
                    HwMod32(j, ROWS * COLS);
                goto found;
            }
        }
    }
found:
    Mem_Free(set);
}
