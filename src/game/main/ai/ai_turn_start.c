#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

int Rng_Next(void);
void Rng_SetState(u32 state);
int func_01ff9f7c(void);
void func_0208c9f8(int player);
void func_0208cdf8(void);
void func_0208d914(void);
void func_0208d6f4(void);
void func_0208d56c(void);
void func_0208d548(void);
void func_0208d62c(void);
void func_0208d964(void);
void func_0208d2f0(void);
int func_0209dec0(void);
int func_0209de14(void);

/* Sets up an AI turn: picks the player's first unit, reseeds the RNG,
 * runs the setup routines, then starts the unit phases. */
void AiTurn_Start(void)
{
    Unk_02080440_48 *state;

    data_027c027c->unk_48[data_027c0284]->actingUnitIndex =
        Ov000_gPlayerFirstUnitSlot[data_027c027c->unk_48[data_027c0284]->currentPlayer];
    state = data_027c027c->unk_48[data_027c0284];
    state->actingUnit = &state->units[state->actingUnitIndex];

    Rng_SetState(Rng_Next() + func_01ff9f7c());

    func_0208c9f8(data_027c027c->unk_48[data_027c0284]->currentPlayer);
    func_0208cdf8();
    func_0208d914();
    func_0208d6f4();
    func_0208d56c();
    func_0208d548();
    func_0208d62c();
    func_0208d964();
    func_0208d2f0();

    if (func_0209dec0() == 1) {
        gAiTurnState->flags |= 4;
    } else {
        gAiTurnState->flags &= ~4;
    }
    if (func_0209de14() == 1) {
        gAiTurnState->flags |= 8;
    } else {
        gAiTurnState->flags &= ~8;
    }
    gAiTurnState->phase = 0;
    gAiTurnState->unk_01 = 0;
    gAiTurnState->state = AI_STATE_RUN_PHASE;
}
