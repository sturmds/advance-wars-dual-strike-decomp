#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_UpdateTouch(void);
void Ov006_UpdateMapEditor(void);
void Ov006_UpdateArmyCoPanel(void);
void func_ov006_02353f7c(void);
void func_ov006_02351aa4(void);
void Ov006_RefreshSlots(void);
void Ov006_StartScriptAfterDelay(void);
void Task_ResumeScript(Task* task);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern int data_0229eae4;

/* Dispatches on the request posted by
 * Ov006_PostRequest (likely a per-frame task callback; nothing calls it directly), then copies cursorX and cursorY to prevCursorX and prevCursorY, clears
 * flag 2, and when flag 0x4000 is set resumes the script task unk_5c. */
void DesignRoom_Update(void) {
  Ov006_UpdateTouch();
  switch (data_ov006_0236d640->request) {
    case 0:
      Ov006_StartScriptAfterDelay();
      break;
    case 1:
      Ov006_UpdateMapEditor();
      break;
    case 2:
      Ov006_UpdateArmyCoPanel();
      break;
    case 3:
      func_ov006_02353f7c();
      break;
    case 5:
      Ov006_RefreshSlots();
      break;
    case 7:
      break;
  }
  func_ov006_02351aa4();
  data_ov006_0236d640->prevCursorX = data_ov006_0236d640->cursorX;
  data_ov006_0236d640->prevCursorY = data_ov006_0236d640->cursorY;
  data_ov006_0236d640->unk_00 &= ~2;
  if (data_ov006_0236d640->unk_00 & 0x4000) {
    Task_ResumeScript(data_ov006_0236d640->unk_5c);
    data_0229eae4 = 0;
  }
}
