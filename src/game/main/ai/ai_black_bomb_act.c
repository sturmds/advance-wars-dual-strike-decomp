#include "types.h"

void func_0209b2b0(int arg);
void AiBlackBomb_MoveTowardEnemy(void);

/* Queue routine for Black Bombs, set by AiAction_QueueBlackBombs and
 * AiAction_QueueBlackBombsForEagle. */
void AiBlackBomb_Act(void) {
  func_0209b2b0(0);
  AiBlackBomb_MoveTowardEnemy();
}
