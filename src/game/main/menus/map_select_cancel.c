#include "types.h"
#include "game/map_select.h"

u32 Link_IsActive(void);
void EventQueue_Push(int id, int arg);
/* Cancel handler: undoes the first pick of a two-map battle if there is one,
 * otherwise leaves the screen (MapSelect_Leave). In link mode the request is
 * queued as event 8 or 6 instead. */
void MapSelect_CancelFirstMap(void);
void MapSelect_Leave(void);

extern MenuStateFrom400 data_0229ef30;

void MapSelect_Cancel(void) {
  if (data_0229ef30.firstMapId != 0) {
    if (Link_IsActive() == 1) {
      EventQueue_Push(8, 0);
    } else {
      MapSelect_CancelFirstMap();
    }
  } else {
    if (Link_IsActive() == 1) {
      EventQueue_Push(6, 0);
    } else {
      MapSelect_Leave();
    }
  }
}
