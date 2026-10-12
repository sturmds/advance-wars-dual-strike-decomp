#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

void MapSelect_LoadWarRoomRecords(void);
void MapSelect_DrawRecordMarkers(int list, int page);

/* Starts the second slide of the map select (the strip at TABLES.subStrip),
 * 16 frames, and blocks the two arrow buttons until it ends. Reloads the
 * War Room records and redraws the page markers. */
void MapSelect_StartSubSlide(int slideBack) {
  MENU->setup.subSlideBack = slideBack;
  MENU->setup.subSlideFrames = 16;
  MenuButton_DisableTouch(249);
  MenuButton_DisableTouch(248);
  MapSelect_LoadWarRoomRecords();
  MapSelect_DrawRecordMarkers(LIST_OF_TAB(TAB()), PAGE());
}
