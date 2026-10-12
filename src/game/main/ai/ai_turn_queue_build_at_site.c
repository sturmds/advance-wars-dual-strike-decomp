#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/unit_action.h"

u8 AiBuild_RollOrder(u8 unitType, int arg);

/* Common tail of the AiTurn_TryBuild* functions once gUnitAction.target holds
 * the unit type. Passes the type to AiBuild_RollOrder (guess: it picks a value the
 * action carries in unk_07), marks the site as used and remembers its index. */
void AiTurn_QueueBuildAtSite(int index) {
  gUnitAction.unk_07 = AiBuild_RollOrder(gUnitAction.target, 0);
  data_0229e5bc[index].kind = SITE_KIND_SKIPPED;
  data_021619c0->unk_07++;
  data_021619c0->buildSite = index;
  data_021619c0->buildStep = 1;
}
