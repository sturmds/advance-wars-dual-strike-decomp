#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void ResultScreen_Confirm(MenuButton* button);
void ResultPath_DrawAll(int lead);

extern u8 data_027c0060[];

/* Script step: draws the path sprites and the data_027c0060 sprite at rest
 * and confirms the result screen on A or Start. */
void ResultSprite_Hold(void) {
  ResultPath_DrawAll(195 - (5 - data_022a7a9c.rank) * 35);
  Sprite_QueueAt(6, 200, 88, &data_027c0060, 0x1000, 0x40a);
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
    ResultScreen_Confirm(NULL);
  }
}
