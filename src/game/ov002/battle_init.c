#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_LoadBattleUnitData(int unitType1, int unitType2);
void func_ov002_02350b24(void);
void func_ov002_02350c8c(int unitType1, int unitType2);
void func_ov002_02351254(void);
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
    func_ov002_02350b24();
    func_ov002_02350c8c(BATTLE_SIDE_UNIT_TYPE(0), BATTLE_SIDE_UNIT_TYPE(1));
    func_ov002_02351254();
    Battle_InitHpDisplay();
    func_ov002_023571d0();
    func_ov002_023571b8();
    func_ov002_0235323c();
}
