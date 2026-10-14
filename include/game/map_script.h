#ifndef GAME_MAP_SCRIPT_H
#define GAME_MAP_SCRIPT_H

#include "types.h"
#include "game/event_script.h"

/* What MapHeader.script points to: trigger lists (EventTrigger, run by
 * EventTriggers_Run) for the situations a campaign map reacts to. Which
 * situation each list belongs to is known only from its runner; the runners
 * are named by list offset. A map without a list for a situation points to
 * a list that holds only EVENT_TRIGGER_END, or has NULL. */
typedef struct MapScript {
  const EventTrigger*
      list00; /* run by Game_State6_PlayThemeAndScripts when a turn starts (after the player theme) */
  const EventTrigger* list04; /* run by MapScript_RunList04 (game state 11) */
  const EventTrigger*
      list08; /* run with a player 1 unit by MapScript_RunList08 (tile selected) */
  const EventTrigger*
      list0c; /* run with the unit by MapScript_RunList0c (action finished) */
  const EventTrigger*
      list10; /* run with an id and a unit by MapScript_RunList10 */
  const EventTrigger* list14; /* run by Game_EnterState20 */
  const EventTrigger* list18; /* run by Game_TickPlayerTimer with a flag */
} MapScript;

typedef char MapScript_size_check[sizeof(MapScript) == 0x1c ? 1 : -1];

#endif
