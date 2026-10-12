#include "types.h"
#include "game/ai_unit_action.h"

BOOL EventScript_IsRunning(void);
void AiTurn_Start(void);
void AiTurn_RunPhase(void);
void AiTurn_RunQueue(void);
void AiTurn_ShowAction(void);
void AiTurn_RunEndActions(void);
void AiTurn_RunPowerStep(void);
void AiTurn_Restart(void);

/* Runs one step of the AI turn. Does nothing while EventScript_IsRunning
 * returns nonzero. */
void AiTurn_Update(void) {
  if (EventScript_IsRunning()) {
    return;
  }
  switch (gAiTurnState->state) {
    case AI_STATE_START:
      AiTurn_Start();
      break;
    case AI_STATE_RUN_PHASE:
      AiTurn_RunPhase();
      break;
    case AI_STATE_RUN_QUEUE:
      AiTurn_RunQueue();
      break;
    case AI_STATE_SHOW_ACTION:
      AiTurn_ShowAction();
      break;
    case AI_STATE_END_ACTIONS:
      AiTurn_RunEndActions();
      break;
    case 5:
      AiTurn_RunPowerStep();
      break;
    case 6:
      AiTurn_Restart();
      break;
  }
}
