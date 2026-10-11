#include "types.h"
#include "game/event_queue.h"
#include "game/map_select.h"

u8 Map_IsFlagSet(u32 mapId);
u32 func_0207bd80(void);
void func_0205d594(void);
void MapSelect_ConfirmMap(void);

extern MenuStateFrom400 data_0229ef30;

/* Position of the selected map on its page. */
#define PAGE_POS() (TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()])

/* Picks the selected map. Maps below id 0x101 must be unlocked
 * (Map_IsFlagSet). The first pick of a two-map battle outside the War Room
 * goes to func_0205d594; otherwise MapSelect_ConfirmMap takes the map.
 * In link mode (func_0207bd80 returns 1) the pick is queued as event 7 or 5
 * with the map's page position instead. */
void MapSelect_PickMap(void)
{
    if (SELECTED_MAP_ID() < 0x101 && !Map_IsFlagSet(SELECTED_MAP_ID())) {
        return;
    }
    if (STATE->gameMode != GAME_MODE_WAR_ROOM && STATE->twoMaps == 1 && data_0229ef30.unk_4fc == 0) {
        if (func_0207bd80() == 1) {
            EventQueue_Push(7, PAGE_POS());
        } else {
            func_0205d594();
        }
    } else {
        if (func_0207bd80() == 1) {
            EventQueue_Push(5, PAGE_POS());
        } else {
            MapSelect_ConfirmMap();
        }
    }
}
