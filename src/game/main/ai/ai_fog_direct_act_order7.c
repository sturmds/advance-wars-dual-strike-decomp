#include "types.h"

void Ai_ClearThreatMaps(void);
void AiUnit_AttackAdjacent(void);

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with AI order 7. Ai_ClearThreatMaps clears the per-tile
 * AI maps (MapTiles.threatMax and threatSum). */
void AiFogDirect_ActOrder7(void) {
  Ai_ClearThreatMaps();
  AiUnit_AttackAdjacent();
}
