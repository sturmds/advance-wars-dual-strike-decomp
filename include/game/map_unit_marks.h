#ifndef GAME_MAP_UNIT_MARKS_H
#define GAME_MAP_UNIT_MARKS_H

/* Marks the 3D map draws on a unit, as bits of the word Map3d_DrawTileUnit
 * builds for Map3D_DrawUnit. The names follow the condition that sets
 * each bit; which icon each one shows is not checked yet. */
#define UNIT_MARK_HIDDEN_HP    0x001 /* the owner has SKILL_HIDDEN_HP and is not in vision grid 1 */
#define UNIT_MARK_LOW_FUEL     0x002 /* fuel at most half of the type's maximum */
#define UNIT_MARK_LOW_AMMO     0x004 /* ammo at most half of the type's maximum */
#define UNIT_MARK_PARALYSED    0x008
#define UNIT_MARK_DIVED        0x010
#define UNIT_MARK_CAPTURING    0x020
#define UNIT_MARK_CARRYING     0x040
#define UNIT_MARK_HIDDEN_CARGO 0x080 /* UnitType_HidesCargo */
#define UNIT_MARK_WAITED       0x100 /* Map3D_DrawUnit then uses another palette */

#endif
