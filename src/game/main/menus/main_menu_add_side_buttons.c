#include "types.h"

void MenuButton_Add(int, int, void*, int);
extern u8 data_02159a9c[];
extern u8 data_02159ac4[];

void MainMenu_AddSideButtons(void) {
  MenuButton_Add(0, 0, data_02159a9c, 253);
  MenuButton_Add(0, 0, data_02159ac4, 254);
}
