#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ai_unit_action.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/unit_action.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
int func_020b9664(void);
void func_020b81c4(void);
void func_020b8230(void);
void func_020b82a0(void);
void func_020b8308(void);
void func_020b8370(int arg);
void func_020b8964(void);
void func_020b8a24(void);
void func_020b8ad4(void);
void func_020b8b80(void);
void func_020b8c70(void);
void func_020b8cb0(void);
void func_020c8888(void);
void func_020ca398(void);
void func_020dd404(void);
void func_020dd4c4(void);
void func_020dd550(void);
void func_020dd6bc(void);
void func_020dd818(void);
void func_020dd89c(void);
void func_020dd9a0(void);
void func_020ddc2c(void);
void func_020ddd30(void);
void func_020de350(int arg);
void func_020de440(void);
void func_020de5a4(void);
void func_020deac4(void);

/* Second display step: runs the display routine of the action in
 * gUnitAction. Unless the action is 1, 13 to 21 or 27, the acting unit
 * first gets its final tile and is marked as waited. Most actions end by
 * going back to the queue; 4, 5, 8 and 15 and a failed 22 set the next
 * state themselves. Action 27 sets the AI behaviour of a player in both
 * game states. */
void AiShowAction_Step2(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (!(gUnitAction.action == 1 || gUnitAction.action == 14 || gUnitAction.action == 13
          || gUnitAction.action == 15 || gUnitAction.action == 16 || gUnitAction.action == 17
          || gUnitAction.action == 18 || gUnitAction.action == 19 || gUnitAction.action == 20
          || gUnitAction.action == 21 || gUnitAction.action == 27)) {
        GAME->actingUnit->flags |= MAP_UNIT_WAITED;
        GAME->actingUnit->x = GAME->actingUnitDestX;
        GAME->actingUnit->y = GAME->actingUnitDestY;
        func_020c8888();
    }
    switch (gUnitAction.action) {
    case 1:
        func_020b8cb0();
        break;
    case 14:
        func_020b8c70();
        break;
    case 13:
        func_020b8b80();
        break;
    case 15:
        func_020dd6bc();
        if (func_020b9664() == 1) {
            gAiTurnState->state = AI_STATE_RUN_QUEUE;
        } else if (GAME->players[GAME->currentPlayer].tagPowerHalf == TAG_POWER_BACK) {
            gAiTurnState->state = AI_STATE_START;
        } else {
            gAiTurnState->state = 6;
        }
        gAiTurnState->actionStep = 0;
        return;
    case 16:
        func_020dd550();
        break;
    case 17:
        func_020dd4c4();
        break;
    case 18:
        func_020dd404();
        break;
    case 2:
        func_020deac4();
        break;
    case 3:
        func_020de5a4();
        func_020deac4();
        break;
    case 4:
        func_020b8ad4();
        return;
    case 5:
        func_020b8a24();
        return;
    case 7:
        func_020de440();
        func_020deac4();
        break;
    case 8:
        func_020b8370(1);
        func_020b8370(0);
        gAiTurnState->actionStep = 10;
        return;
    case 10:
        func_020ddd30();
        func_020deac4();
        break;
    case 6:
        func_020ddc2c();
        break;
    case 11:
        func_020b8308();
        func_020deac4();
        break;
    case 12:
        func_020b82a0();
        func_020deac4();
        break;
    case 20:
        func_020b8230();
        break;
    case 21:
        func_020ca398();
        break;
    case 22:
        if (GAME->gameMode == 0 && GAME->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 18) {
            func_020dd9a0();
            func_020deac4();
        } else {
            func_020b8964();
            return;
        }
        break;
    case 23:
        func_020b81c4();
        break;
    case 24:
        func_020de350(gUnitAction.target - 1);
        func_020deac4();
        break;
    case 25:
        func_020dd89c();
        func_020deac4();
        break;
    case 26:
        func_020deac4();
        func_020dd818();
        break;
    case 27:
        saved0280 = data_027c0280;
        saved027c = data_027c027c;
        Screen_SetCurrentBoth(data_027c0288.unk_00);
        GAME->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
        Screen_SetCurrentBoth(data_027c0288.unk_04);
        GAME->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        break;
    }
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
    gAiTurnState->actionStep = 0;
}
