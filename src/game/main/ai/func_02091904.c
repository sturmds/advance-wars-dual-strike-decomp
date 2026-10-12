#include "types.h"

void AiBuild_SumUnitHp(void);
int AiTurn_PickUnitToBuild(int arg);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 9. AiTurn_PickUnitToBuild returns -1 when it finds nothing, else an
 * index that AiTurn_QueueBuildAtSite takes (the other steps pass it the same way). What
 * either does is not known yet. */
BOOL func_02091904(void) {
  int index;

  AiBuild_SumUnitHp();
  index = AiTurn_PickUnitToBuild(1);
  if (index == -1) {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
