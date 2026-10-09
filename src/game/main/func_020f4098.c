#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_020f35e0(int value);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s32 data_02169ba4[][5];
extern s32 data_02169ba8[][5];

/* Looks up the map id of the first screen's game state in a table of 20-byte
 * entries (map id, value, ...; ends at map id -1) and passes the value, or 0,
 * to func_020f35e0. Only in game mode 0. */
void func_020f4098(void)
{
    int value;
    int i;
    int gameMode;
    int mapId;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    gameMode = data_027c027c->unk_48[data_027c0284]->gameMode;
    mapId = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId;
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    value = 0;
    if (gameMode == 0) {
        for (i = 0; data_02169ba4[i][0] != -1; i++) {
            if (mapId == data_02169ba4[i][0]) {
                value = data_02169ba8[i][0];
                break;
            }
        }
    }
    func_020f35e0(value);
}
