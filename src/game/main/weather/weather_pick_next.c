#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

int Rng_RollTier(u32 limit1, u32 limit2, u32 limit3);
BOOL func_020d2bfc(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define VIEW    (data_027c027c->unk_48[data_027c0284]->unk_1c4)
#define WEATHER (data_027c027c->unk_5c[data_027c0284])

/* Weather for the next turn. Clear weather stays clear unless the map's
 * weather setting is random (unk_3b is 1); then unk_02, unk_01 and unk_00 of
 * the weather record are percent chances of snow, rain and sand. Other
 * weather lasts until func_020d2bfc reports that every player's days ran
 * out, then returns to the map's default (unk_3d). */
int Weather_PickNext(void)
{
    int weather = VIEW->weather;

    if (weather == WEATHER_CLEAR) {
        if (VIEW->unk_3b != 1) {
            return weather;
        }
        switch (Rng_RollTier(WEATHER->unk_02, WEATHER->unk_02 + WEATHER->unk_01,
                             WEATHER->unk_00 + (WEATHER->unk_02 + WEATHER->unk_01))) {
        case 1:
            weather = WEATHER_SNOW;
            break;
        case 2:
            weather = WEATHER_RAIN;
            break;
        case 3:
            weather = WEATHER_SAND;
            break;
        }
    } else {
        if (weather == VIEW->unk_3d && VIEW->unk_3b == 0) {
            return weather;
        }
        if (func_020d2bfc()) {
            weather = VIEW->unk_3d;
        }
    }
    return weather;
}
