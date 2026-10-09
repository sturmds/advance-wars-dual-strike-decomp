#ifndef GAME_UNIT_INFO_PANEL_H
#define GAME_UNIT_INFO_PANEL_H

#include "types.h"
#include "game/map_unit.h"

/* State of the unit info panel in ov000, reached through
 * data_ov000_0234ec8c. The panel shows one unit with a frame in the
 * owner's army palette and sprite set. */
typedef struct UnitInfoPanel {
    u8 unk_00;                         /* 120 when shown by UnitInfoPanel_OpenForType and func_ov000_022b1f90; passed to the drawing code */
    u8 pad_01[0x04 - 0x01];
    MapUnit *unit;                     /* unit shown, or the scratch copy in data_ov000_02350510 */
    u8 owner;                          /* player slot of the unit */
    u8 palette;                        /* Player.armyColour - 1 */
    u8 style;                          /* Player_GetArmyStyle - 1 */
    u8 buffer;                         /* flipped each time func_ov000_022b2628 draws the panel */
} UnitInfoPanel;

extern UnitInfoPanel *data_ov000_0234ec8c;

#endif
