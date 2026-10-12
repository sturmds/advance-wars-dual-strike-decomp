#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai_site.h"
#include "game/ai_unit_action.h"
#include "game/event_script.h"
#include "game/map_unit.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const EventScriptCommand gMapScrollScript[];
extern u32 gLinkPlay;

void func_020f5878(int x, int y);
s16 ScriptSlot_Find(const EventScriptCommand* script);
MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType);
int Ai_CountFootUnitsByUnk0c4(int skipSlot, int flag);
int GetUnitMaxRange(int player, int unitType);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020b81c0(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Action number sent for the build; AiShowAction_Step2 runs
 * AiShowAction_BuildUnitAlt for it. */
#define ACTION_BUILD_AT_SITE 14

/* End-actions build at the AiSite build->buildSite, in two calls: the first
 * scrolls the map to the site, the next ones wait for the scroll script to end,
 * then buy gUnitAction.target there with aiOrder gUnitAction.unk_07 and no
 * targets. unk_0c_4 of the new unit: Infantry and Mech fill the profile's
 * quotas of units with it set (unk_08[1]) and clear (unk_08[0]) first; then
 * units with no weapon set it, the rest clear it when the roll is below the
 * unit type's unk_05. Returns FALSE when no build is in progress. */
BOOL AiBuild_BuyAtSite(void) {
  MapUnit* unit;
  AiBuildState* build = data_021619c0;
  AiSite* site = &data_0229e5bc[build->buildSite];

  switch (build->buildStep) {
    case 1:
      build->buildStep++;
      func_020f5878(site->x, site->y);
      break;
    case 2:
      if (ScriptSlot_Find(gMapScrollScript) != -1) {
        break;
      }
      data_021619c0->buildStep = 0;
      unit = CurrentPlayer_BuyUnit(site->x, site->y, gUnitAction.target);
      unit->aiOrder = gUnitAction.unk_07;
      unit->serviceWanted = 0;
      unit->transportWanted = 0;
      unit->aiTargetX = MAP_UNIT_NO_TARGET;
      unit->aiTargetY = MAP_UNIT_NO_TARGET;
      if (unit->type == UNIT_INFANTRY || unit->type == UNIT_MECH) {
        if (Ai_CountFootUnitsByUnk0c4(unit - STATE->units, 1) <
            gAiProfile->unk_08[1]) {
          unit->unk_0c_4 = 1;
          goto done;
        }
        if (Ai_CountFootUnitsByUnk0c4(unit - STATE->units, 0) <
            gAiProfile->unk_08[0]) {
          unit->unk_0c_4 = 0;
          goto done;
        }
      }
      if (GetUnitMaxRange(STATE->currentPlayer, unit->type) == 0) {
        unit->unk_0c_4 = 1;
      } else if (gAiTurnState->roll <
                 gAiProfile->units[unit->type - 1].unk_05) {
        unit->unk_0c_4 = 0;
      } else {
        unit->unk_0c_4 = 1;
      }
    done:
      STATE->actingUnitDestX = site->x;
      STATE->actingUnitDestY = site->y;
      if (gLinkPlay != 0) {
        UnitAction_Send(ACTION_BUILD_AT_SITE, gUnitAction.target,
                        gUnitAction.unk_07, 0);
      }
      func_020b81c0(unit);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}
