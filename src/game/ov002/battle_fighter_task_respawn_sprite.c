#include "types.h"
#include "game/battle_fighter_sprites.h"
#include "game/battle_fighter_task.h"

void Task_Destroy(void *task);
int Battle_GetMatchupNibble1(int side);

#define MENU_BYTES ((u8 *)&gMenuState)

/* Points the task at the fighter of its side and replaces the fighter's second
 * sprite task with a new one made from the side's table (gMenuState.battle.unk_30),
 * placed and scaled like the fighter. Does nothing to the sprites when the side has
 * no table. */
void BattleFighterTask_RespawnSprite(BattleFighterTask *task)
{
    u8 side;
    u16 scale;
    int unk_56;
    BOOL left;
    int unk_58;
    BattleFighter *fighter;

    task->fighter = (BattleFighter *)BATTLE_ENTRY(task->side, 0).unk_00;
    task->hitList = (BattleHitList *)(MENU_BYTES + BATTLE_OFFSET(hitLists)) + task->side;
    task->cells = BATTLE_SIDE_DATA(task->side)->cellPositions;
    if (*(u32 *)(MENU_BYTES + task->side * 4 + BATTLE_OFFSET(unk_30)) != 0) {
        if (BATTLE_FIGHTER_ENTRY(task->fighter).sprites[1] != 0) {
            Task_Destroy(BATTLE_FIGHTER_ENTRY(task->fighter).sprites[1]);
            BATTLE_FIGHTER_ENTRY(task->fighter).sprites[1] = 0;
        }
        left = (task->fighter->side == 0);
        /* Needed to match: the original loads task->fighter again here. */
        fighter = *(BattleFighter *volatile *)&task->fighter;
        scale = fighter->unk_44;
        unk_58 = left ? 0 : 2;
        unk_56 = left ? 0 : 2;
        side = fighter->side;
        BATTLE_FIGHTER_ENTRY(fighter).sprites[1] = BattleSpriteTask_Create(
            *(const u32 **)(MENU_BYTES + task->fighter->side * 4 + BATTLE_OFFSET(unk_30)),
            Battle_GetMatchupNibble1(task->side), fighter->unk_38, fighter->unk_3a, fighter->unk_3c, 0,
            fighter->unk_46, 0, scale, scale, *(u16 *)(MENU_BYTES + side * 2 + 4), unk_56, 0, side,
            fighter->unk_58_0, fighter->unk_3e, fighter->unk_40, fighter->unk_42, unk_58);
        task->sprite = BATTLE_FIGHTER_ENTRY(task->fighter).sprites[1];
    }
    task->counter = 0;
}
