#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void TileCodeTable_Load(void);
void Ov006_NewMap(void);
void Panels_PlaceForCursor(void);

#define OV6 data_ov006_0236d640

/* Resets the map view for the Design Room. With keep 0 it also puts the
 * scroll back to the top left and calls Ov006_NewMap. */
void Ov006_ResetMapView(int keep) {
  MAP_STATE->unk_18 = 0;
  TileCodeTable_Load();
  if (keep == 0) {
    MAP_STATE->unk_1c4->scrollX = 0;
    MAP_STATE->unk_1c4->scrollY = 0;
    MAP_STATE->unk_1c4->unk_22 = 0;
    MAP_STATE->unk_1c4->unk_24 = 0;
    MAP_STATE->unk_1c4->unk_26 = 0;
    MAP_STATE->unk_1c4->unk_28 = 0;
    Ov006_NewMap();
  }
  MAP_STATE->cursorX = 0;
  MAP_STATE->cursorY = 0;
  MAP_STATE->cursorPixelX = 0;
  MAP_STATE->cursorPixelY = 0;
  OV6->viewX = 0;
  OV6->viewY = 0;
  Panels_PlaceForCursor();
}
