#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/menu_button.h"

extern EventScriptCommand data_0216923c[];
extern MenuButtonTemplate data_02158f54;

void func_020ef110(void) {
  EventScript_Start(data_0216923c);
  data_027c027c->unk_48[data_027c0284]->cursorX =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_027c027c->unk_48[data_027c0284]->cursorY =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  MenuButton_AddReserved(0, 0, &data_02158f54);
}
