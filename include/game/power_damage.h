#ifndef GAME_POWER_DAMAGE_H
#define GAME_POWER_DAMAGE_H

#include "types.h"
#include "game/weather.h"

/* Picks which enemy units a damage power hits; non-zero means hit.
 * The argument holds a map x at +2 and y at +3. */
typedef BOOL (*PowerDamageFilter)(const void* unit);

/* Task made from gPowerDamageTaskDesc by StartPowerDamage. The first 0x29 bytes
 * belong to the task system (Task_Create). */
typedef struct PowerDamageTask {
  u8 pad_00[0x29];
  u8 unitIndex;  /* next unit to hit, 1-50 within playerSlot's units */
  u8 playerSlot; /* player slot whose units are being hit */
  u8 delay;      /* frames since the last hit */
  u8 player;     /* index of the player using the power */
  u8 damage;     /* to enemy units, 10 per displayed HP */
  u8 heal;       /* to own units, 10 per displayed HP */
  u8 weather;    /* WEATHER_* */
  PowerDamageFilter filter; /* NULL for powers that hit every enemy */
  BOOL halveFuel;           /* set by Drake's powers, which halve enemy fuel */
  s16 blendFrame;           /* frame counter of the screen blend steps */
} PowerDamageTask;

/* PowerDamageTask.unitIndex runs from 1 up to this, one unit per hit. */
#define POWER_DAMAGE_UNIT_END 51

/* PowerDamageTask.delay: frames waited between two hits. */
#define POWER_DAMAGE_HIT_DELAY 4

/* Child task of PowerDamageTask, made from the descriptor the power
 * passes in. The two names look like effect file names ("1f4"). */
typedef struct PowerEffectTask {
  u8 pad_00[0x4c];
  const char* effectA;
  const char* effectB;
} PowerEffectTask;

void StartPowerDamage(const void* effectDesc, const char* effectA,
                      const char* effectB, int player, int damage, int heal,
                      int weather, BOOL halveFuel, PowerDamageFilter filter,
                      void* parent);

#endif
