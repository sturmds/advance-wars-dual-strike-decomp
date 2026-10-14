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

/* Where UnitInfoPanel_DrawPageMarker draws the marker of a panel page
 * (data_ov000_0234f9e8, one per page): the arguments of
 * Sprite_DrawPulsingCorners, x from the panel's left edge. */
typedef struct UnitInfoPanelMarker {
  u8 x;
  u8 y;
  u8 width; /* pages 4 and 11 use the width of a description text instead */
  u8 height;
} UnitInfoPanelMarker;

typedef char
    UnitInfoPanelMarker_size_check[sizeof(UnitInfoPanelMarker) == 4 ? 1 : -1];

extern UnitInfoPanel
    data_ov000_02350520; /* what data_ov000_0234ec8c points to */

/* Pages of the panel (UnitInfoPanel.page). data_ov000_0234fa28 has a row
 * per unit type group (data_ov000_022b3850[type] - 1) and the text id of
 * each page in it, 0 for a page the group does not have. */
#define UNIT_INFO_PANEL_PAGES 16
extern u32 data_ov000_0234fa28[][UNIT_INFO_PANEL_PAGES];

/* Tile blocks of layer 0 that the panel clears before drawing: the page's
 * text area and the picture block. */
#define UNIT_INFO_TEXT_COLUMNS 14
#define UNIT_INFO_TEXT_ROWS 8
#define UNIT_INFO_PICTURE_COLUMNS 15
#define UNIT_INFO_PICTURE_ROWS 20

#endif
