#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueSmallNumber(int list, u16 x, int y, u32 value);
int GetUnitMaxRange(int player, int unitType);

extern u32 data_ov000_0234eef0;

/* Draws ammo against the type's maximum ammo on the second screen, and for a unit
 * whose longest range is above 1 an icon with the minimum and maximum range.
 * Nothing is drawn for types without ammo. A one-digit number shifts 4 pixels left
 * (the range shift is 3 when the maximum has two digits). The separator sprites
 * 0xf36a and 0xf36b are guesses. */
void UnitInfoPanel_DrawAmmoAndRange(void) {
  MapUnit* unit = data_ov000_0234ec8c->unit;
  int ammo = unit->ammo;
  int maxAmmo = Ov000_gUnitStats[unit->type].ammo;
  int minRange = Ov000_gUnitStats[unit->type].minRange;
  int maxRange = GetUnitMaxRange(data_ov000_0234ec8c->owner, unit->type);
  Unk_027c0280* savedGfx = data_027c0280;
  int shift;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  if (Ov000_gUnitStats[unit->type].ammo != 0) {
    Sprite_QueueSmallNumber(
        0, data_ov000_0234ec8c->originX + 80 + (unit->ammo < 10 ? -4 : 0), 88,
        ammo);
    Sprite_QueueSmallNumber(
        0,
        data_ov000_0234ec8c->originX + 89 +
            (Ov000_gUnitStats[unit->type].ammo < 10 ? -4 : 0),
        96, maxAmmo);
    Sprite_QueueAt(0, data_ov000_0234ec8c->originX + 81, 92,
                   &data_ov000_0234eef0, 0xf36a, 0);
    if (maxRange > 1) {
      Icon_Draw(63, data_ov000_0234ec8c->originX + 100, 88, 0, 0);
      shift = maxRange < 10 ? 0 : 3;
      Sprite_QueueSmallNumber(0, data_ov000_0234ec8c->originX + 104 - shift, 96,
                              minRange);
      Sprite_QueueAt(0, data_ov000_0234ec8c->originX + 112 - shift, 97,
                     &data_ov000_0234eef0, 0xf36b, 0);
      Sprite_QueueSmallNumber(0, data_ov000_0234ec8c->originX + 120 + shift, 96,
                              maxRange);
    }
  }
  Screen_SetCurrentGfx(savedGfx);
}
