#include "types.h"
#include "game/unit_info_panel.h"

void UnitInfoPanel_DrawPictureAndStats(u8 a, MapUnit* unit, u8 palette,
                                       u8 style);
void UnitInfoPanel_DrawUnitName(u8 a, MapUnit* unit);
void Gfx_RequestLayerCopy(int layer);

/* Draws the panel for its current unit and queues the copy of layer 0. */
void UnitInfoPanel_DrawContents(void) {
  UnitInfoPanel_DrawPictureAndStats(
      data_ov000_0234ec8c->originX, data_ov000_0234ec8c->unit,
      data_ov000_0234ec8c->palette, data_ov000_0234ec8c->style);
  UnitInfoPanel_DrawUnitName(data_ov000_0234ec8c->originX,
                             data_ov000_0234ec8c->unit);
  Gfx_RequestLayerCopy(0);
}
