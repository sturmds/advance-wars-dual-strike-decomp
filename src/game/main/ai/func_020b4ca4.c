#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"
#include "game/sound_arc.h"
#include "game/unit_action.h"
#include "sdk/wl.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"

void (*gAiQueueRoutine)(void); /* run on each entry */
int (*data_0229e174)(int a, int b, int c);
u8* gAiQueueCursor; /* write position in gAiQueue */
BOOL (*gAiUnloadTest)(void);
int (*gAiUnloadScore)(void);
AiTurnState data_0229e178;
AiBuildState data_0229e184;
UnitAction gUnitAction;
u8 gAiTransportsSentToUnit[64];
/* sort key of each gAiQueue entry, same index; size: up to the next symbol */
u8 gAiQueueSortKeys[0x44];
u8 gAiQueue[0x44]; /* size: up to the next symbol */
/* per unit type: build score; 0: not a candidate; size: up to the next symbol */
u32 gAiBuildScores[0x1b];
AiProfile data_0229e2e4;
AiSiteGroup gAiSiteGroups[0x5c]; /* size: up to the next symbol */
AiSite data_0229e5bc[0x5c];      /* size: up to the next symbol */

BOOL AiSeaTransport_CargoReachesTarget(void);
int func_020b4d04(void);
void func_020b5464(void);
void func_020b5bf4(void);

void func_020b4ca4(void) {
  gAiUnloadTest = AiSeaTransport_CargoReachesTarget;
  gAiUnloadScore = func_020b4d04;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}
