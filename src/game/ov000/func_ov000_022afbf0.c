#include "types.h"
#include "game/screen_other_map.h"
#include "game/player.h"

typedef struct PlayerCounts {
    int unitCount;
    int properties;
    int inGame;
} PlayerCounts;

extern PlayerCounts data_ov000_023504e0[4];
extern int data_ov000_0234eb84[][5];
extern int data_ov000_0234eb88[][5];

extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
u8 Player_GetArmyStyle(int playerIndex);
u8 Map_GetLoadedPlayerCount(void);
void Font_LoadDigits(int baseTile);
void func_0201e6e8(int a, int b, int c, int d);
void func_ov000_022af8f0(int a, int b, int player, int style, int properties, int unitCount, int props2, int inGame);

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_ov000_022afbf0(void)
{
    u8 changed;
    int properties;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    Unk_027c0280 *savedGfx2;
    Unk_02080440 *savedScreen2;
    Unk_027c0280 *savedGfx3;
    int i;
    Player *player;
    int j;
    PlayerCounts *counts;
    changed = 0;
    savedGfx = data_027c0280;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    for (i = 1, counts = data_ov000_023504e0; i < 5; i++, counts++) {
        player = &STATE->players[i];
        if (player->controller != 0) {
            properties = player->unk_15;
            if (STATE->unk_81 == 0) {
                if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
                    if ((player->unk_1b & 2) == 0) {
                        properties = -1;
                    }
                }
            }
            if (counts->unitCount != player->unitCount || properties != counts->properties || counts->inGame != Player_IsInGame(i)) {
                counts->unitCount = STATE->players[i].unitCount;
                counts->properties = properties;
                counts->inGame = Player_IsInGame(i);
                changed = 1;
            }
        }
    }
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
    if (changed == 0) {
        return;
    }
    Font_LoadDigits(0);
    savedGfx2 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen2 = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    savedGfx3 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    for (j = 1, counts = data_ov000_023504e0; j < 5; j++, counts++) {
        player = &STATE->players[j];
        if (player->controller != 0) {
            func_ov000_022af8f0(14, data_ov000_0234eb84[Map_GetLoadedPlayerCount()][j], j, Player_GetArmyStyle(j), player->armyColour, counts->unitCount, counts->properties, counts->inGame);
        }
    }
    func_0201e6e8(14, (s16)data_ov000_0234eb88[Map_GetLoadedPlayerCount()][0], 8,
                  (s16)(data_ov000_0234eb84[Map_GetLoadedPlayerCount()][Map_GetLoadedPlayerCount()] + 6 - data_ov000_0234eb88[Map_GetLoadedPlayerCount()][0]));
    Screen_SetCurrentGfx(savedGfx3);
    Screen_SetCurrent(savedScreen2);
    Screen_SetCurrentGfx(savedGfx2);
}
