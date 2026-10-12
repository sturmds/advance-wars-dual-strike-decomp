#include "types.h"

void CallbackSlots_Add(void (*callback)(void));
void MapPreview_CycleBorderColour(void);
void MapPreview_DrawToScreen(u8* charBase, u16* screen, int firstTile,
                             int palette);
BOOL MapPreviewTask_Start(void* charData);

/* Draws the map preview and keeps it animated. Does nothing when the
 * preview task is already running. */
void MapPreview_Start(u8* charBase, u16* screen, int firstTile, int palette) {
  if (!MapPreviewTask_Start(charBase + firstTile * 32)) {
    return;
  }
  CallbackSlots_Add(MapPreview_CycleBorderColour);
  MapPreview_DrawToScreen(charBase, screen, firstTile, palette);
}
