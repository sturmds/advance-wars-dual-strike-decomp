#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleScreenTask.h"
#include "game/BattleSideData.h"
#include "game/battle_hit_list.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

#define MENU ((MenuState*)&gMenuState)
#define DUE_CELL (hitList->flags[hitList->cursor].cell)

void Battle_StartFighterTask(BattleCellTask* cellTask);
void func_ov002_02357e5c(int side, int cell);
void Battle_StartCellEffect(int side, int cell);
void Battle_PutFighterSprite(int side, int cell);
void func_ov002_0235f02c(BattleCellTask* cellTask);
void func_ov002_0235f3c0(BattleCellTask* cellTask);

/* Runs the entries of each side's hit list that are due on this frame. For a side whose
 * BattleSideData.unk_48 is 0 an entry sets step 3 in its cell's task. Otherwise the side starts
 * one animation task, by its unit type, with the cell 0 task. Needed to match: side is u32 but
 * compared as int. */
void BattleCells_RunSequence(BattleScreenTask* screen) {
  BattleCellTask* cellTask;
  u32 side;
  BattleCellTask* firstCell;
  BattleHitList* hitList;

  for (side = 0; (int)side < 2; side++) {
    hitList = &MENU->battle.hitLists[side];
    while (hitList->cursor != hitList->count &&
           screen->frame == hitList->key[hitList->cursor]) {
      if (MENU->battle.unk_27c[side]->unk_48 == 0) {
        cellTask = (BattleCellTask*)BATTLE_CELL_ENTRY(side, DUE_CELL).unk_00;
        cellTask->steps[cellTask->stepIndex] = 3;
        if (MENU->battle.unk_48[side] != 0) {
          Battle_StartCellEffect(side, DUE_CELL);
        }
        cellTask->stepIndex++;
        if (cellTask->stepIndex == 8) {
          cellTask->stepIndex = 0;
        }
      } else if (screen->fighterStarted[side] == 0) {
        firstCell = (BattleCellTask*)BATTLE_CELL_ENTRY(side, 0).unk_00;
        if (Ov002_gBattleScene->sides[side].usesSecondary == 0) {
          switch (BATTLE_SIDE_UNIT_TYPE(side)) {
            case UNIT_BATTLESHIP:
            case UNIT_CARRIER:
              func_ov002_0235f3c0(firstCell);
              screen->fighterStarted[side] = 1;
              break;
            case UNIT_MEGATANK:
            case UNIT_PIPERUNNER:
            case 30:
              func_ov002_0235f3c0(firstCell);
              screen->fighterStarted[side] = 1;
              break;
            case UNIT_CRUISER:
              func_ov002_0235f02c(firstCell);
              screen->fighterStarted[side] = 1;
              break;
            case UNIT_STEALTH:
            case UNIT_BOMBER:
            case UNIT_SUB:
            case 31:
            case 32:
              if (MENU->battle.unk_40[side] != 0) {
                Battle_PutFighterSprite(side, DUE_CELL);
              }
              if (MENU->battle.unk_48[side] != 0) {
                func_ov002_02357e5c(side, DUE_CELL);
              }
              break;
          }
        } else if (BATTLE_SIDE_UNIT_TYPE(side) == UNIT_MEGATANK ||
                   BATTLE_SIDE_UNIT_TYPE(side) == UNIT_CRUISER) {
          Battle_StartFighterTask(firstCell);
          screen->fighterStarted[side] = 1;
        }
      }
      hitList->cursor++;
    }
  }
}
