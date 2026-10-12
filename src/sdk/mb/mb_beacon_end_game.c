#include "types.h"
#include "sdk/mb.h"

extern DlPlayAdvertProgress data_022ace4c;

void MbBeacon_EndGame(void)
{
    data_022ace4c.currentGame->sentPlayerFlags = 1;
    data_022ace4c.changeCount = data_022ace4c.currentGame->changeCount;
    data_022ace4c.state = DLPLAY_ADVERT_STEP_ROSTER;
}
