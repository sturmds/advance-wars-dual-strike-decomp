#include "types.h"

void PopupMenu_PushState(void);
void Ov006_ClearLayer1MenuBox(void);
void func_ov006_0235790c(void);
void func_ov006_02357920(void);

/* Menu item handler (called through a data table). Event 2 runs one script
 * directly; any other event saves the pop-up state, clears the menu box and
 * starts another. */
void Ov006_PopupChoiceStartScriptB(int menu, int item, int event) {
  if (event == 2) {
    func_ov006_0235790c();
    return;
  }
  PopupMenu_PushState();
  Ov006_ClearLayer1MenuBox();
  func_ov006_02357920();
}
