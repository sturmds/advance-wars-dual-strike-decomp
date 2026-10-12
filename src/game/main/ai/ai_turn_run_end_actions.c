#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL AiBuild_BuyAtSite(void);
void AiEndActions_Start(void);
void AiEndActions_BuildThreatMaps(void);
BOOL func_02091eb0(void);
void func_02093720(void);
BOOL AiTurn_TrySiteAction(void);
BOOL AiTurn_TryBuildFighter(void);
BOOL func_02091c54(void);
BOOL AiTurn_TryBuildCruiser(void);
BOOL AiTurn_TryBuildFootUnit(void);
void func_02092134(void);
BOOL func_02090154(void);
int Ai_WantsBlackBomb(int arg);
void func_0209b2b0(int arg);
void AiSite_ScorePiperunnerNeed(void);
BOOL func_02091904(void);
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
        if (STATE->day == 1 && data_021619c0->unk_07 == 0) {
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
        if (STATE->day == 1 || data_021619c0->unk_07 == 0) {
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
        if (STATE->day == 1 || data_021619c0->unk_07 == 0) {
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
        if (STATE->day == 1 || data_021619c0->unk_07 == 0) {
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
