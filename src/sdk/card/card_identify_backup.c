/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void OS_SleepThread(OSThreadQueue* queue);
void func_021154ac(void);
void CardSave_FillGeometryFromType(int type);
int CARDi_Request(CARDiCommon* common, int command, int retries);
void CardSave_FinishTaskAndNotify(CARDiCommon* common);

/* Sets the backup chip geometry from its type and asks the other CPU to identify the chip. */
void CARD_IdentifyBackup(int type) {
  CARDiCommon* common = &data_022abb00;
  int state;

  func_021154ac();
  state = OS_DisableInterrupts();
  if (common->flag & CARD_STAT_BUSY) {
    do {
      OS_SleepThread(&common->queue);
    } while (common->flag & CARD_STAT_BUSY);
  }
  common->flag |= CARD_STAT_BUSY;
  OS_RestoreInterrupts(state);

  CardSave_FillGeometryFromType(type);
  common->callback = NULL;
  common->callbackArg = NULL;
  data_022abb00.waiter = OSi_ThreadInfo.current;
  CARDi_Request(common, 2, 1);
  CardSave_FinishTaskAndNotify(common);
}
