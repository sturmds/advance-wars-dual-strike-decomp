#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

#define PICKS (&data_0229c73c->coGrid[(column - 1) * 2])
#define MAP_CO_PICKED 0x1c /* MapHeader.co: the player picks */

int Campaign_IsHardMode(void);
void MovementCost(void);
void Player_SetCo(int column, int row, int co);
void func_020e7a80(int gameMode, int mapId);

void func_02044c94(void)
{
    int column;
    int row;
    int co;

    if (STATE->gameMode != 0 || Campaign_IsHardMode() == 1) {
        for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
            for (row = 0; row < data_0229c73c->rowCount; row++) {
                STATE->playerCos[column][row] = PICKS[row];
                Player_SetCo(column, row, PICKS[row]);
            }
        }
    } else {
        for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
            for (row = 0; row < data_0229c73c->rowCount; row++) {
                co = Ov000_gMapHeaders[STATE->unk_1c4->mapId].co[column][row];
                if (co == MAP_CO_PICKED) {
                    STATE->playerCos[column][row] = PICKS[row];
                    Player_SetCo(column, row, PICKS[row]);
                } else {
                    STATE->playerCos[column][row] = co;
                    Player_SetCo(column, row, Ov000_gMapHeaders[STATE->unk_1c4->mapId].co[column][row]);
                }
            }
        }
    }

    if (STATE->gameMode == 3) {
        return;
    }
    func_020e7a80(STATE->gameMode, STATE->unk_1c4->mapId);
    MovementCost();

    if (STATE->secondMapId == 0) {
        return;
    }
    if (OtherMap_GetUnk30() != 0) {
        return;
    }
    {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;

        Screen_SetCurrentGfx((&data_027c0288.unk_00)[1]);
        saved027c = data_027c027c;
        Screen_SetCurrent((&data_027c0288.unk_00)[1]);
        MovementCost();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
}
