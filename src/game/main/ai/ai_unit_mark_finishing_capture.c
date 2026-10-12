#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

int Unit_CaptureCompletes(MapUnit* unit);

/* Called by AiUnit_PerformAction. For a capture by a unit with more than
 * 60 hp that would take the property this turn, sets MapUnit.unk_0c_5. What
 * the flag does is unknown; the name is a guess. */
void AiUnit_MarkFinishingCapture(int action) {
  if (action != AI_ACTION_CAPTURE) {
    return;
  }
  if (GAME->actingUnit->hp < 61) {
    return;
  }
  if (Unit_CaptureCompletes(GAME->actingUnit) != TRUE) {
    return;
  }
  GAME->actingUnit->unk_0c_5 = 1;
}
