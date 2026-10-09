#ifndef GAME_POWER_DAMAGE_H
#define GAME_POWER_DAMAGE_H

#include "types.h"
#include "game/weather.h"

/* Picks which enemy units a damage power hits; non-zero means hit.
 * The argument holds a map x at +2 and y at +3. */
typedef BOOL (*PowerDamageFilter)(const void *unit);

/* Task made from gPowerDamageTaskDesc by StartPowerDamage. The first 0x2c bytes
 * belong to the task system (Task_Create). */
typedef struct PowerDamageTask {
    u8 pad_00[0x2c];
    u8 player;                         /* index of the player using the power */
    u8 damage;                         /* to enemy units, 10 per displayed HP */
    u8 heal;                           /* to own units, 10 per displayed HP */
    u8 weather;                        /* WEATHER_* */
    PowerDamageFilter filter;          /* NULL for powers that hit every enemy */
    BOOL halveFuel;                    /* set by Drake's powers, which halve enemy fuel */
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
