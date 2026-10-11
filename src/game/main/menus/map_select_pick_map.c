#include "types.h"
#include "game/event_queue.h"
#include "game/map_select.h"

u8 Map_IsFlagSet(u32 mapId);
u32 Link_IsActive(void);
void MapSelect_PickFirstMap(void);
void MapSelect_ConfirmMap(void);

extern MenuStateFrom400 data_0229ef30;

/* Position of the selected map on its page. */
#define PAGE_POS() (TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()])

/* Picks the selected map. Maps below id 0x101 must be unlocked
 * (Map_IsFlagSet). The first pick of a two-map battle outside the War Room
 * goes to MapSelect_PickFirstMap; otherwise MapSelect_ConfirmMap takes the map.
 * In link mode (Link_IsActive returns 1) the pick is queued as event 7 or 5
 * with the map's page position instead. */
void MapSelect_PickMap(void)
{
    if (SELECTED_MAP_ID() < 0x101 && !Map_IsFlagSet(SELECTED_MAP_ID())) {
        return;
    }
    if (STATE->gameMode != GAME_MODE_WAR_ROOM && STATE->twoMaps == 1 && data_0229ef30.firstMapId == 0) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(7, PAGE_POS());
        } else {
            MapSelect_PickFirstMap();
        }
    } else {
        if (Link_IsActive() == 1) {
            EventQueue_Push(5, PAGE_POS());
        } else {
            MapSelect_ConfirmMap();
        }
    }
}
