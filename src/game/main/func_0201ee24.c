#include "types.h"

void PopupMenu_CallItemHandler(int a0);
void PopupMenu_DrawItemIcons(int);
int PopupMenu_AddButtons(int);

int func_0201ee24(int a0) {
  PopupMenu_CallItemHandler(a0);
  PopupMenu_DrawItemIcons(a0);
  return PopupMenu_AddButtons(a0);
}
