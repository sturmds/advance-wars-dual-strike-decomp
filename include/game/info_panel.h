#ifndef GAME_INFO_PANEL_H
#define GAME_INFO_PANEL_H

#include "types.h"

/* The map screen's info panel: a terrain box (defense stars, capture
 * points or tile hp), a unit box (icon, hp, fuel, ammo) and a cargo box
 * for the unit under the cursor. */

/* Offset of one item from its box's corner, in pixels. */
typedef struct InfoPanelPos {
  u8 x;
  u8 y;
} InfoPanelPos;

/* data_021663f4: where each item sits inside its box. */
typedef struct InfoPanelLayout {
  InfoPanelPos unitIcon; /* unit box */
  InfoPanelPos unk_02;
  InfoPanelPos weaponIcon;
  InfoPanelPos hp;
  InfoPanelPos fuel;
  InfoPanelPos ammo;
  InfoPanelPos cargoOnly;  /* cargo box, one unit aboard */
  InfoPanelPos cargoFirst; /* cargo box, two units aboard */
  InfoPanelPos cargoSecond;
  InfoPanelPos unk_12[2];
  InfoPanelPos terrainIcon; /* terrain box: capture or hp icon */
  InfoPanelPos defenseStars;
  InfoPanelPos terrainCount; /* capture points left or tile hp */
} InfoPanelLayout;

extern InfoPanelLayout data_021663f4;

/* Side of the screen the panel sits on. */
#define INFO_PANEL_LEFT 0
#define INFO_PANEL_RIGHT 1
#define INFO_PANEL_SIDES 2

/* Panel x and y at rest on each side. A panel x at or left of the left
 * one counts as the left side. */
typedef struct InfoPanelRestPos {
  u16 x;
  s16 y;
} InfoPanelRestPos;

extern InfoPanelRestPos gInfoPanelRestPos[INFO_PANEL_SIDES];

/* [side][unit boxes shown]: x of window 0's left edge from the panel's x
 * plus 1. */
extern s16 gInfoPanelWindowLeft[INFO_PANEL_SIDES][3];

/* Two digits of value (0-99) as sprites, the ones digit at x. */
void InfoPanel_DrawNumber(u16 x, u16 y, s16 value);

/* A unit's sprite in the panel; slot 0 for the unit box, 1 and 2 for
 * cargo. */
void InfoPanel_DrawUnit(s16 x, s16 y, int type, int owner, u8 carrying,
                        u8 captureProgress, u8 flags, int slot);

#endif
