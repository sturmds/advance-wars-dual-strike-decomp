#include "types.h"
#include "game/screen_other_map.h"
#include "game/player.h"
#include "game/player_panel_cache.h"

extern int data_ov000_0234eb84[][5];
extern int data_ov000_0234eb88[][5];

extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
u8 Player_GetArmyStyle(int playerIndex);
u8 Map_GetLoadedPlayerCount(void);
void Font_LoadDigits(int baseTile);
void func_0201e6e8(int a, int b, int c, int d);
void SecondScreen_DrawPlayerRow(int a, int b, int player, int style, int properties, int unitCount, int props2, int inGame);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Compares each player's unit count, property count and in-game flag with
 * Ov000_gPlayerPanelCache and, when any changed, redraws all player rows on
 * the second screen. The property count shows as -1 under fog or rain unless
 * bit 1 of visionGrids is set. */
void PlayerPanels_Refresh(void)
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
    PlayerPanelCache *cache;
    int j;
    changed = 0;
    savedGfx = data_027c0280;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    for (i = 1; i < 5; i++) {
        player = &STATE->players[i];
        cache = &Ov000_gPlayerPanelCache[i];
        if (player->controller != PLAYER_CONTROLLER_NONE) {
            properties = player->propertyCount;
            if (STATE->propertyGoal == 0) {
                if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
                    if ((player->visionGrids & 2) == 0) {
                        properties = -1;
                    }
                }
            }
            if (cache->unitCount != player->unitCount || properties != cache->properties || cache->inGame != Player_IsInGame(i)) {
                cache->unitCount = STATE->players[i].unitCount;
                cache->properties = properties;
                cache->inGame = Player_IsInGame(i);
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
    for (j = 1; j < 5; j++) {
        player = &STATE->players[j];
        cache = &Ov000_gPlayerPanelCache[j];
        if (player->controller != PLAYER_CONTROLLER_NONE) {
            SecondScreen_DrawPlayerRow(14, data_ov000_0234eb84[Map_GetLoadedPlayerCount()][j], j, Player_GetArmyStyle(j), player->armyColour, cache->unitCount, cache->properties, cache->inGame);
        }
    }
    func_0201e6e8(14, (s16)data_ov000_0234eb88[Map_GetLoadedPlayerCount()][0], 8,
                  (s16)(data_ov000_0234eb84[Map_GetLoadedPlayerCount()][Map_GetLoadedPlayerCount()] + 6 - data_ov000_0234eb88[Map_GetLoadedPlayerCount()][0]));
    Screen_SetCurrentGfx(savedGfx3);
    Screen_SetCurrent(savedScreen2);
    Screen_SetCurrentGfx(savedGfx2);
}
