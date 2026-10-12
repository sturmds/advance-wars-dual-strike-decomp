#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/menu_button.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern EventScriptCommand data_02168fd8[];
extern MenuButtonTemplate data_02158f7c;

/* Starts the script at data_02168fd8, points the view target (cursorX, cursorY)
 * at the acting unit's destination tile and adds a menu button. Purpose
 * unknown. */
void func_020ebecc(void) {
  EventScript_Start(data_02168fd8);
  data_027c027c->unk_48[data_027c0284]->cursorX =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_027c027c->unk_48[data_027c0284]->cursorY =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  MenuButton_AddReserved(0, 0, &data_02158f7c);
}
