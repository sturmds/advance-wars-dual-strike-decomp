#include "types.h"

void PopupMenu_PushState(void);
void Ov006_ClearLayer1MenuBox(void);
void func_ov006_02357770(void);

/* Menu item handler (called through a data table). Unless the event is 2,
 * saves the pop-up state and starts a script. What event 2 means is not known. */
void Ov006_PopupChoiceStartScriptA(int menu, int item, int event) {
  if (event == 2) {
    return;
  }
  PopupMenu_PushState();
  Ov006_ClearLayer1MenuBox();
  func_ov006_02357770();
}
