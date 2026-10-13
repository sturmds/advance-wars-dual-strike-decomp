#ifndef GAME_UNIT_INFO_PANEL_H
#define GAME_UNIT_INFO_PANEL_H

#include "types.h"
#include "game/map_unit.h"

/* State of the unit info panel in ov000, reached through
 * data_ov000_0234ec8c. The panel shows one unit with a frame in the
 * owner's army palette and sprite set. */
typedef struct UnitInfoPanel {
  u8 originX; /* x in pixels everything is drawn from; 120 from UnitInfoPanel_OpenForType and UnitInfoPanel_OpenForUnit */
  u8 pad_01[0x02 - 0x01];
  u8 tileX; /* top left of the panel text area, in tiles; the text starts one tile in */
  u8 tileY;
  MapUnit* unit; /* unit shown, or the scratch copy in data_ov000_02350510 */
  u8 owner;      /* player slot of the unit */
  u8 palette;    /* Player.armyColour - 1 */
  u8 style;      /* Player_GetArmyStyle - 1 */
  u8 buffer; /* flipped each time UnitInfoPanel_DrawPictureAndStats draws the panel */
  s8 page;      /* page shown; 0 from UnitInfoPanel_StartTask */
  s8 shownPage; /* page drawn last; -1 from UnitInfoPanel_StartTask */
} UnitInfoPanel;

extern UnitInfoPanel* data_ov000_0234ec8c;
extern MapUnit
    data_ov000_02350510; /* scratch unit for UnitInfoPanel_OpenForType */

#endif
