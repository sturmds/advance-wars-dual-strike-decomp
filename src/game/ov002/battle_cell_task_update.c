#include "types.h"
#include "game/BattleCellTask.h"

void BattleCellTask_RunStep(BattleCellTask* task, int step);
void BattleFighter_StepFootHop(BattleCellTask* task);
void BattleFighter_StepSlideMode0(BattleCellTask* task);
void BattleFighter_StepSlideAirSeaMode1(BattleCellTask* task);

/* Per-frame callback of the cell task script: runs the steps queued in the steps ring, then
 * the task's per-frame routines, and counts the frame in unk_50. */
void BattleCellTask_Update(BattleCellTask* task) {
  while (task->unk_5a_lo != task->stepIndex) {
    BattleCellTask_RunStep(task, task->steps[task->unk_5a_lo]);
    task->unk_5a_lo++;
    if (task->unk_5a_lo == 8) {
      task->unk_5a_lo = 0;
    }
  }
  BattleCellTask_Bob(task);
  BattleCellTask_Move(task);
  BattleCellTask_UpdateShadow(task);
  BattleCellTask_Flash(task);
  BattleFighter_StepFootHop(task);
  BattleFighter_StepSlideMode0(task);
  BattleFighter_StepSlideAirSeaMode1(task);
  BattleCellTask_RestartSprite(task);
  (*(u16*)&task
        ->unk_50)++; /* needed to match: unk_50 is typed s16 in BattleCellTask; the original reads it as u16 */
}
