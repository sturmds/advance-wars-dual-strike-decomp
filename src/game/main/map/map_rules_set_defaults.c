#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/play_records.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void MapRules_SetDefaultTeams(void); /* team[1..4] = 0, 1, 2, 3 */
void Map_SetCoSelectPhase(u8 value);
void func_0200a630(void); /* data_027c0284 = 0 */

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Resets the rules of the current state before SetupMapRules applies a
 * map: four human players in armies 1-4 on their own teams, default CO
 * pairs, 1000 funds per property, no fog and clear weather on both screen
 * states (data_027c0288), tag partner skills on for everyone. */
void MapRules_SetDefaults(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    STATE->unk_10 = 0;
    STATE->unk_14 = 0;
    STATE->slotControllers[1] = PLAYER_CONTROLLER_HUMAN;
    STATE->slotControllers[2] = PLAYER_CONTROLLER_HUMAN;
    STATE->slotControllers[3] = PLAYER_CONTROLLER_HUMAN;
    STATE->slotControllers[4] = PLAYER_CONTROLLER_HUMAN;
    STATE->army[1] = 1;
    STATE->army[2] = 2;
    STATE->army[3] = 3;
    STATE->army[4] = 4;
    STATE->playerCos[1][0] = CO_JAKE;
    STATE->playerCos[2][0] = CO_SASHA;
    STATE->playerCos[3][0] = CO_JAVIER;
    STATE->playerCos[4][0] = CO_GRIMM;
    STATE->playerCos[1][1] = CO_RACHEL;
    STATE->playerCos[2][1] = CO_COLIN;
    STATE->playerCos[3][1] = CO_EAGLE;
    STATE->playerCos[4][1] = CO_SENSEI;
    STATE->unk_aa[0] = 0;
    STATE->unk_aa[1] = 0;
    STATE->unk_aa[2] = 0;
    STATE->unk_aa[3] = 0;
    STATE->unk_aa[4] = 0;
    STATE->unk_aa[5] = 0;
    STATE->unk_aa[6] = 0;
    STATE->unk_aa[7] = 0;
    MapRules_SetDefaultTeams();
    STATE->unk_7c = 0;
    STATE->unk_80 = 0;
    STATE->propertyGoal = 0;
    STATE->fundsPerProperty = 1000;
    STATE->unk_74 = 0;
    STATE->unk_64 = 0;
    STATE->unk_68 = 0;
    STATE->unk_6c = 0;
    STATE->unk_70 = 0;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    STATE->unk_1c4->fog = 0;
    STATE->unk_1c4->tileset = 0;
    STATE->unk_1c4->defaultWeather = 0;
    STATE->unk_1c4->weather = WEATHER_CLEAR;
    STATE->unk_1c4->randomWeather = 0;
    STATE->unk_1c4->nextWeather = 0;
    STATE->unk_1c4->unk_37 = 3;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    STATE->unk_1c4->fog = 0;
    STATE->unk_1c4->tileset = 0;
    STATE->unk_1c4->defaultWeather = 0;
    STATE->unk_1c4->weather = WEATHER_CLEAR;
    STATE->unk_1c4->randomWeather = 0;
    STATE->unk_1c4->nextWeather = 0;
    STATE->unk_1c4->unk_37 = 3;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    STATE->unk_84 = 1;
    STATE->luckOnRequestOnly = 0;
    STATE->unk_8c = 2;
    STATE->unk_86 = 0;
    STATE->secondMapId = 0;
    STATE->coEffectsOn = 1;
    STATE->unk_5c = 1;
    STATE->partnerBoostHuman = 1;
    STATE->partnerBoostAi = 1;
    STATE->unk_82 = gPlayRecords.unk_12;
    STATE->musicReady = gPlayRecords.unk_0e;
    Map_SetCoSelectPhase(0);
    func_0200a630();
}
