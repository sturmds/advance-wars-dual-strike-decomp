#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_LoadBattleUnitData(int unitType1, int unitType2);
void Battle_PickSideVariants(void);
void Battle_SelectUnitRecords(int unitType1, int unitType2);
void Battle_LoadScreen(void);
void Battle_InitHpDisplay(void);
void func_ov002_023571d0(void);
void func_ov002_023571b8(void);
void func_ov002_0235323c(void);

/* Sets up the battle screen after the scene is loaded; the argument is not used. */
int Battle_Init(int unused)
{
    int i;

    for (i = 0; i < 2; i++) {
        gMenuState.battle.unk_408[i] = 0;
        gMenuState.battle.unk_40c[i] = 0;
    }
    Sound_LoadBattleUnitData(BATTLE_SIDE_UNIT_TYPE(0), BATTLE_SIDE_UNIT_TYPE(1));
    Battle_PickSideVariants();
    Battle_SelectUnitRecords(BATTLE_SIDE_UNIT_TYPE(0), BATTLE_SIDE_UNIT_TYPE(1));
    Battle_LoadScreen();
    Battle_InitHpDisplay();
    func_ov002_023571d0();
    func_ov002_023571b8();
    func_ov002_0235323c();
}
