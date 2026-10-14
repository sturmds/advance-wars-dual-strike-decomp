#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* The AI unit action steps, one per phase: without fog, and with fog. */
extern void (*gAiUnitPhases[])(void);
extern void (*gAiUnitPhasesFog[])(void);

/* Runs the step for the current phase, then moves to the next phase. */
void AiTurn_RunPhase(void) {
  Unk_02080440_1c4* map = data_027c027c->unk_48[data_027c0284]->unk_1c4;

  /* Rain brings fog too. */
  if (map->fog == 0 && map->weather != WEATHER_RAIN) {
    gAiUnitPhases[gAiTurnState->phase]();
  } else {
    gAiUnitPhasesFog[gAiTurnState->phase]();
  }
  gAiTurnState->phase++;
}
