#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_build_units.h"
#include "game/ai/ai_sites.h"
#include "game/ai/ai_build_buy.h"
#include "game/ai/ai_end_actions.h"
#include "game/ai/ai_threat_maps.h"
#include "game/ai/ai_build_pick.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

void func_02093720(void);
void func_020b81a4(void);
void func_020b81b8(void);

/* AI_STATE_END_ACTIONS: runs the ten steps in AiBuildState.step, each in
 * its own function, one after another within a frame until a step returns
 * nonzero (it needs more frames). Afterwards, or when unk_07 reaches 50,
 * the AI state becomes 5 (see AiTurn_Update). The steps are not decompiled
 * yet, so what each does is unknown. */
void AiTurn_RunEndActions(void) {
  if (AiBuild_BuyAtSite() == 1) {
    return;
  }
  for (;;) {
    if (data_021619c0->step != 0 && data_021619c0->unk_07 >= 50) {
      gAiTurnState->state = 5;
      func_020b81b8();
      return;
    }
    switch (data_021619c0->step) {
      case 0:
        AiEndActions_Start();
        data_021619c0->step++;
        AiEndActions_BuildThreatMaps();
        break;
      case 1:
        if (func_02091eb0()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 2:
        if (MAP_STATE->day == 1 && data_021619c0->unk_07 == 0) {
          data_021619c0->step++;
          break;
        }
        if (AiTurn_TrySiteAction()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 3:
        if (MAP_STATE->day == 1 || data_021619c0->unk_07 == 0) {
          data_021619c0->step++;
          break;
        }
        if (AiTurn_TryBuildFighter()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 4:
        if (MAP_STATE->day == 1 || data_021619c0->unk_07 == 0) {
          data_021619c0->step++;
          break;
        }
        if (func_02091c54()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 5:
        if (MAP_STATE->day == 1 || data_021619c0->unk_07 == 0) {
          data_021619c0->step++;
          break;
        }
        if (AiTurn_TryBuildCruiser()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 6:
        if (AiTurn_TryBuildFootUnit()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      case 7:
        func_02092134();
        data_021619c0->step++;
        break;
      case 8:
        if (func_02090154()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        if (Ai_WantsBlackBomb(0) == 1) {
          func_0209b2b0(1);
        }
        AiSite_ScorePiperunnerNeed();
        break;
      case 9:
        if (func_02091904()) {
          return;
        }
        data_021619c0->step++;
        func_02093720();
        break;
      default:
        func_020b81a4();
        gAiTurnState->state = 5;
        return;
    }
  }
}

/* Clears AiSiteGroup.unk_01 in every group. Purpose unknown. */
void func_020937dc(void) {
  int i;
  AiSiteGroup* slot = gAiSiteGroups;

  for (i = 0; i < data_021619c0->slotCount; i++, slot++) {
    slot->unk_01 = 0;
  }
}

/* Sets AiSite.kind back to 127 on every site marked 0xfe (no action
 * found) or 0xff (rejected for a unit type). */
void AiSites_ClearAllMarks(void) {
  int i;
  AiSite* site = data_0229e5bc;

  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= 254) {
      site->kind = 127;
    }
  }
}

/* Sets AiSite.kind back to 127 on every site that AiTurn_PickUnitToBuild
 * marked 0xff while trying out a unit type. */
void AiSites_ClearRejectedMarks(void) {
  int i;
  AiSite* site;

  site = data_0229e5bc;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind == 0xff) {
      site->kind = 127;
    }
  }
}
