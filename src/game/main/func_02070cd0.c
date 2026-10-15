#include "types.h"
#include "game/menu_button.h"

void TabList_RebuildButtons(void);

void func_02070cd0(void) {
  MenuButtons_EnableTouch();
  TabList_RebuildButtons();
}
