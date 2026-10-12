#include "types.h"

void MenuButton_Add(int, int, void*, int);
int SurvivalMode_HasSecondRow(void);
extern u8 data_0215ac9c[];

void func_02046dc4(void) {
  MenuButton_Add(0, 32, data_0215ac9c, 0);
  if (SurvivalMode_HasSecondRow() == 1) {
    MenuButton_Add(0, 96, data_0215ac9c, 1);
  }
}
