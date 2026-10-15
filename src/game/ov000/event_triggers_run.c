/* The original was built without optimization: every MAP_STATE read reloads both
 * globals, and the arguments are stored on the stack on entry. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/map_unit.h"
#include "game/unlocks.h"

extern Unk_02080440_48 data_02183b08;
extern Unk_02080440_48 data_02189cd8;

int Campaign_IsHardMode(void);
void EventScript_StartOrQueue(const EventScriptCommand* script);
const EventTrigger* EventTriggers_SkipBlock(const EventTrigger* trigger);

/* Skips the rest of the block when a condition fails. */
#define FAIL_IF(cond)                           \
  if (cond) {                                   \
    trigger = EventTriggers_SkipBlock(trigger); \
    break;                                      \
  }

/* Kind base applies only on data_02183b08, base + EVENT_KIND_SECOND_MAP
 * only on the other state. */
#define FAIL_IF_WRONG_STATE(base)                                 \
  FAIL_IF(trigger->kind == (base) && MAP_STATE == &data_02189cd8) \
  FAIL_IF(trigger->kind == (base) + EVENT_KIND_SECOND_MAP &&      \
          MAP_STATE == &data_02183b08)

/* Walks a map's trigger list (EventTrigger). For each block whose
 * conditions all hold: with run, starts its script and sets its done flag;
 * without, only reports it. unit and id are what the UNIT_TYPE and ID
 * conditions compare with. Returns TRUE if any block fired. */
BOOL EventTriggers_Run(const EventTrigger* trigger, MapUnit* unit, int id,
                       BOOL run) {
  BOOL (*test)(void);
  BOOL fired = FALSE;
  BOOL matched;

  for (; trigger->kind != EVENT_TRIGGER_END; trigger++) {
    switch (trigger->kind) {
      case EVENT_TRIGGER_DAY:
      case EVENT_TRIGGER_DAY + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_DAY + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_DAY)
        matched = FALSE;
        if ((trigger->day == EVENT_TRIGGER_NO_DAY ||
             (trigger->day == 0 && !MAP_STATE->day) ||
             trigger->day == MAP_STATE->day) &&
            (trigger->param == 0 ||
             trigger->param == MAP_STATE->currentPlayer)) {
          matched = TRUE;
        }
        FAIL_IF(!matched)
        break;
      case EVENT_TRIGGER_RUN:
      case EVENT_TRIGGER_RUN + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_RUN + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_RUN)
        /* a block with a done flag fires once */
        FAIL_IF(trigger->param != EVENT_TRIGGER_NO_FLAG &&
                GameFlag_IsSet(trigger->param))
        if (!run) {
          fired = TRUE;
          break;
        }
        if (trigger->u.script) {
          EventScript_StartOrQueue(trigger->u.script);
          fired = TRUE;
        }
        if (trigger->param != EVENT_TRIGGER_NO_FLAG) {
          GameFlag_Set(trigger->param, 1);
        }
        break;
      case EVENT_TRIGGER_TEST_TRUE:
      case EVENT_TRIGGER_TEST_TRUE + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_TEST_TRUE + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_TEST_TRUE)
        test = trigger->u.test;
        FAIL_IF(test() != TRUE)
        break;
      case EVENT_TRIGGER_TEST_FALSE:
      case EVENT_TRIGGER_TEST_FALSE + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_TEST_FALSE + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_TEST_FALSE)
        test = trigger->u.test;
        FAIL_IF(test() != FALSE)
        break;
      case EVENT_TRIGGER_UNIT_TYPE:
      case EVENT_TRIGGER_UNIT_TYPE + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_UNIT_TYPE + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_UNIT_TYPE)
        FAIL_IF(unit->type != trigger->param)
        break;
      case EVENT_TRIGGER_ID:
      case EVENT_TRIGGER_ID + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_ID + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_ID)
        FAIL_IF(trigger->param != id)
        break;
      case EVENT_TRIGGER_FLAG_SET:
      case EVENT_TRIGGER_FLAG_SET + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_FLAG_SET + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_FLAG_SET)
        FAIL_IF(!GameFlag_IsSet(trigger->param))
        break;
      case EVENT_TRIGGER_FLAG_CLEAR:
      case EVENT_TRIGGER_FLAG_CLEAR + EVENT_KIND_SECOND_MAP:
      case EVENT_TRIGGER_FLAG_CLEAR + EVENT_KIND_BOTH_MAPS:
        FAIL_IF_WRONG_STATE(EVENT_TRIGGER_FLAG_CLEAR)
        FAIL_IF(GameFlag_IsSet(trigger->param))
        break;
      case EVENT_TRIGGER_NORMAL_MODE:
        FAIL_IF(Campaign_IsHardMode())
        break;
      case EVENT_TRIGGER_HARD_MODE:
        FAIL_IF(Campaign_IsHardMode() != TRUE)
        break;
    }
  }
  return fired;
}
