#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/strike_target.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_0229e19e[2];
extern void* data_0229eacc[2];
extern u16 data_021658c8[];
int data_0229eac4;
StrikeUnitScoreFunc gStrikeUnitScore;
StrikeSiteScoreFunc gStrikeSiteScore;
extern int data_022a6dd4;

void* UnitShowTask_CreateMoving(int x, int y, int owner, int unitType,
                                void* arg);
void ActingUnit_UnloadCargo(int cargoSlot);
void UnitType_PlayUnloadSound(int unitType);

#define GAME (data_027c027c->unk_48[data_027c0284])
/* player index 0-3 of the acting unit: its slot in units divided by 64 */
#define OWNER_INDEX(s) \
  ((((u32)(((s)->actingUnit - (s)->units) & 0xc0)) << 10) >> 16)

/* Part of the display of action 8 (unload; Step2 calls it for slot 1, then 0).
 * Starts a task that shows the unit in cargo slot cargoSlot of the acting
 * unit and keeps it in data_0229eacc. The previous value in data_0229e19e
 * picks what the task gets: 0 none, 5 the packed values of the state, else
 * the entry of data_021658c8 for that value. Only the last case also calls
 * ActingUnit_UnloadCargo, counts in data_0229eac4 and calls UnitType_PlayUnloadSound. The
 * meaning of those is not known. */
void AiShowAction_ShowCargoUnit(int cargoSlot) {
  int saved = data_0229e19e[cargoSlot];
  Unk_02080440_48* state;
  MapUnit* cargo;

  data_0229eacc[cargoSlot] = NULL;
  if (saved == 5) {
    state = GAME;
    cargo = &state->units[state->actingUnit->cargo[cargoSlot]];
    state->pathSteps[0] = 10;
    GAME->pathSteps[1] = 4; /* needed to match: GAME is read again */
    state = GAME;
    data_0229eacc[cargoSlot] = UnitShowTask_CreateMoving(
        (u16)state->actingUnitDestX, (u16)state->actingUnitDestY,
        OWNER_INDEX(state), cargo->type, state->pathSteps);
    return;
  }
  if (saved == 0) {
    return;
  }
  data_022a6dd4 = saved - 1;
  state = GAME;
  cargo = &state->units[state->actingUnit->cargo[cargoSlot]];
  data_0229eacc[cargoSlot] = UnitShowTask_CreateMoving(
      (u16)state->actingUnitDestX, (u16)state->actingUnitDestY,
      OWNER_INDEX(state), cargo->type, &data_021658c8[saved - 1]);
  ActingUnit_UnloadCargo(cargoSlot);
  data_0229eac4++;
  UnitType_PlayUnloadSound(cargo->type);
}
