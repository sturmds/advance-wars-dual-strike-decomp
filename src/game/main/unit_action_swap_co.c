#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290718.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_021684f8[];
extern u8 data_02168578[];

void func_020e2ce0(int player);
void func_020e19b4(int player);
void *Task_Create(void *script, int level);

#define GAME (data_027c027c->unk_48[data_027c0284])

void UnitAction_SwapCo(void)
{
    func_020e2ce0(GAME->currentPlayer);
    switch (GAME->players[GAME->currentPlayer].tagPowerHalf) {
    case 0:
        if (GAME->players[GAME->currentPlayer].controller == PLAYER_CONTROLLER_HUMAN) {
            if (data_02290718.coSwaps < 999999) {
                data_02290718.coSwaps++;
            }
        }
        func_020e19b4(GAME->currentPlayer);
        Task_Create(data_021684f8, 3);
        break;
    case TAG_POWER_FRONT:
        if (GAME->players[GAME->currentPlayer].controller == PLAYER_CONTROLLER_HUMAN) {
            if (data_02290718.coSwaps < 999999) {
                data_02290718.coSwaps++;
            }
        }
        GAME->players[GAME->currentPlayer].tagPowerHalf = TAG_POWER_BACK;
        func_020e19b4(GAME->currentPlayer);
        Task_Create(data_02168578, 3);
        break;
    case TAG_POWER_BACK:
        break;
    }
}
