#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
void func_020b5604(void);
void func_020b5464(void);
void func_020b5268(void);
void func_020b5138(void);
void func_020b501c(void);
void func_020b4e24(void);
void func_020b4ca4(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CARGO_UNIT(slot) (&STATE->units[STATE->actingUnit->cargo[slot]])

/* Moves the acting sea transport when it carries a unit. The cargo slots
 * are first ordered, with AI_STEP_SWAP_CARGO set to undo it afterwards:
 * a foot unit in slot 1 goes before a vehicle in slot 0, and an APC in
 * slot 0 goes behind whatever is in slot 1. Then the unit in slot 0
 * picks the routine: foot units and APCs use func_020b5604, the others
 * go by their AI order, like AiUnit_ActByOrder. */
void AiSeaTransport_MoveLoaded(void) {
  u8 swap;
  u8 first;
  MapUnit* unit;

  if (STATE->actingUnit->cargo[1] != 0 && CARGO_UNIT(1)->type <= UNIT_MECH &&
      CARGO_UNIT(0)->type > UNIT_MECH) {
    swap = STATE->actingUnit->cargo[0];
    STATE->actingUnit->cargo[0] = STATE->actingUnit->cargo[1];
    STATE->actingUnit->cargo[1] = swap;
    gAiTurnState->stepFlags |= AI_STEP_SWAP_CARGO;
  }
  first = STATE->actingUnit->cargo[0];
  unit = &STATE->units[first];
  if (unit->type == UNIT_APC && STATE->actingUnit->cargo[1] != 0) {
    STATE->actingUnit->cargo[0] = STATE->actingUnit->cargo[1];
    STATE->actingUnit->cargo[1] = first;
    /* needed to match: unit is read before the flag store, so the store
         * makes case 6 below read the state again */
    unit = CARGO_UNIT(0);
    gAiTurnState->stepFlags |= AI_STEP_SWAP_CARGO;
  }
  if (unit->type <= UNIT_MECH || unit->type == UNIT_APC) {
    func_020b5604();
  } else {
    switch (unit->aiOrder) {
      case 0:
      case 1:
        func_020b5464();
        break;
      case 2:
        if (func_0209dd7c() == TRUE) {
          func_020b5268();
        } else {
          func_020b5464();
        }
        break;
      case 3:
      case 5:
        func_020b5138();
        break;
      case 4:
        func_020b501c();
        break;
      case 6:
        if (Player_HasHq(STATE->currentPlayer) == TRUE) {
          func_020b4e24();
        } else {
          func_020b5138();
        }
        break;
      case 8:
      case 9:
      case 10:
        func_020b4ca4();
        break;
    }
  }
  if (gAiTurnState->stepFlags & AI_STEP_SWAP_CARGO) {
    swap = STATE->actingUnit->cargo[0];
    STATE->actingUnit->cargo[0] = STATE->actingUnit->cargo[1];
    STATE->actingUnit->cargo[1] = swap;
    gAiTurnState->stepFlags &= ~AI_STEP_SWAP_CARGO;
  }
}
