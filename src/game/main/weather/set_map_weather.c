#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Makes `weather` the map's current and default weather without starting
 * it (Weather_Change does). unk_3b is cleared. */
void SetMapWeather(int weather)
{
    data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_3d = weather;
    data_027c027c->unk_48[data_027c0284]->unk_1c4->weather = weather;
    data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_3c = weather;
    data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_3b = 0;
}
