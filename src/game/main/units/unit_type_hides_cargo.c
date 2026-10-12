#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/weather.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when fog or rain hides what a transport of this type carries from
 * the player: the owner lacks visionGrids bit 1 and the type has cargo slots.
 * viewer is not read. */
BOOL UnitType_HidesCargo(int type, int viewer, int owner) {
  if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
    if ((STATE->players[owner].visionGrids & 2) == 0) {
      if (Ov000_gUnitStats[type].carryTypes != NULL) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
