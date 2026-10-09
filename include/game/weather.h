#ifndef GAME_WEATHER_H
#define GAME_WEATHER_H

/* Weather values as CO powers pass them. Weather_Set takes them, turns
 * WEATHER_POWER_SNOW into 1 (snow) and picks a longer duration for it. */
#define WEATHER_CLEAR      0
#define WEATHER_RAIN       2
#define WEATHER_POWER_SNOW 0xfe
#define WEATHER_KEEP       (-1)        /* StartPowerDamage only: no change; stored as 0xff */

#endif
