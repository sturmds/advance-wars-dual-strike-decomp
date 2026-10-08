#ifndef GAME_WEATHER_H
#define GAME_WEATHER_H

/* Weather values as CO powers pass them. func_020d2aa8 takes them, turns
 * WEATHER_POWER_SNOW into 1 (snow) and picks a longer duration for it. */
#define WEATHER_RAIN       2
#define WEATHER_POWER_SNOW 0xfe
#define WEATHER_KEEP       0xff        /* StartPowerDamage only: no change */

#endif
