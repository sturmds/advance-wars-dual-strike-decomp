#ifndef GAME_POWER_DAMAGE_H
#define GAME_POWER_DAMAGE_H

#include "types.h"

/* Weather values passed to StartPowerDamage. */
#define POWER_WEATHER_NONE 0xff        /* keep the current weather */
#define POWER_WEATHER_RAIN 2
#define POWER_WEATHER_SNOW 0xfe        /* func_020d2aa8 turns 0xfe into 1, snow */

/* Picks which enemy units a damage power hits; non-zero means hit.
 * The argument holds a map x at +2 and y at +3. */
typedef BOOL (*PowerDamageFilter)(const void *unit);

/* Task made from gPowerDamageTaskDesc by StartPowerDamage. The first 0x2c bytes
 * belong to the task system (func_02023a58). */
typedef struct PowerDamageTask {
    u8 pad_00[0x2c];
    u8 player;                         /* index of the player using the power */
    u8 damage;                         /* to enemy units, 10 per displayed HP */
    u8 heal;                           /* to own units, 10 per displayed HP */
    u8 weather;                        /* POWER_WEATHER_* */
    PowerDamageFilter filter;          /* NULL for powers that hit every enemy */
    BOOL halveFuel;                    /* enemy units lose half their fuel */
} PowerDamageTask;

/* Child task of PowerDamageTask, made from the descriptor the power
 * passes in. The two names look like effect file names ("1f4"). */
typedef struct PowerEffectTask {
    u8 pad_00[0x4c];
    const char *effectA;
    const char *effectB;
} PowerEffectTask;

void StartPowerDamage(const void *effectDesc, const char *effectA, const char *effectB,
                      int player, int damage, int heal, int weather, BOOL halveFuel,
                      PowerDamageFilter filter, void *parent);

#endif
