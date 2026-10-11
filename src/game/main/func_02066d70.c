#include "types.h"
#include "game/weather.h"

void VsSetup_RequestTeamScreenBack(int weather);

void func_02066d70(void) {
    VsSetup_RequestTeamScreenBack(WEATHER_CLEAR);
}
