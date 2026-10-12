#include "types.h"
#include "game/ai_unit_action.h"

int AiSeaTransport_ScoreFootUnitsVsProperties(void);
void AiTransport_HeadForUnloadTile(void);
void AiTransport_HeadForFlaggedUnloadTile(void);
void AiTransport_UnloadInReach(void);

/* Run by the sea transport routines when AI_STEP_UNLOAD_SCORED is set, after the
 * tile search. The score function foot unit transports use gets an extra
 * step (AiTransport_HeadForFlaggedUnloadTile). Purpose of the steps not known yet. */
void func_020b5bf4(void) {
  AiTransport_UnloadInReach();
  if (gAiUnloadScore == AiSeaTransport_ScoreFootUnitsVsProperties) {
    AiTransport_HeadForFlaggedUnloadTile();
  }
  AiTransport_HeadForUnloadTile();
}
