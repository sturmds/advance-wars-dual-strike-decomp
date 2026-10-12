#ifndef GAME_MAP_SCRIPT_H
#define GAME_MAP_SCRIPT_H

#include "types.h"
#include "game/event_script.h"

/* What MapHeader.unk_00 points to: trigger lists (EventTrigger, run by
 * EventTriggers_Run) for the situations a campaign map reacts to. Which
 * situation each list belongs to is not known; the runners are named by
 * list offset. */
typedef struct MapScript {
  const EventTrigger*
      list00; /* run by Game_State6_PlayThemeAndScripts when a turn starts (after the player theme) */
  u8 pad_04[0x10 - 0x04];
  const EventTrigger*
      list10; /* run with an id and a unit by MapScript_RunList10 */
  const EventTrigger* list14; /* run by Game_EnterState20 */
  const EventTrigger* list18; /* run by Game_TickPlayerTimer with a flag */
} MapScript;

#endif
