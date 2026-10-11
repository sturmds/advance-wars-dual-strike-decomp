#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void func_020b98bc(void);
void Players_Reset(void);
void MapRules_SetDefaults(void);
void Players_ApplyBattleSetup(void);
void Panels_PlaceForCursor(void);
void Units_ClearAll(void);
void MapObjects_Clear(void);
void CurrentPlayer_SyncState(void);
void CurrentPlayer_StartTurn(void);
void func_020c8888(void);
void ScreenConfig_Select(int a);
void Battle_RegisterFrameCallbacks(void);
void func_01ff9c48(const void *descriptor);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 MainLoop_MapFrame[];
extern u32 gMapGameRunning;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Sets up a four-player battle with fixed COs: Andy, Olaf, Eagle and
 * Kanbei for players 1-4, Nell in the neutral slot. */
void Battle_SetupWithPresetCos(void)
{
    func_020b98bc();
    STATE->unk_1c4->playerCount = 4;
    Players_Reset();
    MapRules_SetDefaults();
    Players_ApplyBattleSetup();

    STATE->players[0].front.id = CO_NELL;
    STATE->players[1].front.id = CO_ANDY;
    STATE->players[2].front.id = CO_OLAF;
    STATE->players[3].front.id = CO_EAGLE;
    STATE->players[4].front.id = CO_KANBEI;

    STATE->unk_18 = 0;
    STATE->cursorX = 0;
    STATE->cursorY = 0;
    STATE->cursorPixelX = 0;
    STATE->cursorPixelY = 0;

    Panels_PlaceForCursor();
    Units_ClearAll();
    MapObjects_Clear();
    CurrentPlayer_SyncState();
    CurrentPlayer_StartTurn();
    func_020c8888();
    ScreenConfig_Select(5);
    if (HOOKS->unk_54 != NULL) {
        HOOKS->unk_54(1);
    }
    Battle_RegisterFrameCallbacks();
    func_01ff9c48(MainLoop_MapFrame);
    gMapGameRunning = 0;
}
