#include "types.h"
#include "game/menu_button.h"

int SurvivalMode_HasSecondRow(void);
extern MenuButtonTemplate data_0215ac9c;

void func_02046dc4(void) {
  MenuButton_Add(0, 32, &data_0215ac9c, 0);
  if (SurvivalMode_HasSecondRow() == 1) {
    MenuButton_Add(0, 96, &data_0215ac9c, 1);
  }
}
