#include "types.h"
#include "game/Unk_02080440.h"

void func_020cfa88(void);
void func_020cfa14(void);
void func_020cf9a0(void);
void func_020cf8e8(void);

/* Calls the routine for one weather (WEATHER_*). */
void Weather_StartEffect(int weather)
{
    switch (weather) {
    case WEATHER_CLEAR:
        func_020cfa88();
        break;
    case WEATHER_SNOW:
        func_020cfa14();
        break;
    case WEATHER_RAIN:
        func_020cf9a0();
        break;
    case WEATHER_SAND:
        func_020cf8e8();
        break;
    }
}
