#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_button.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236b6e8[5]
                             [2]; /* (x, y) of each button when unitMode is 0 */
extern u8 data_ov006_0236b6f4[5][2]; /* the same when unitMode is 1 */
extern const MenuButtonTemplate gOv006ToolButtonTemplate;

#define OV6 data_ov006_0236d640

/* Adds menu buttons 96-99 on the first screen, plus button 100 when unitMode
 * is 0; flag 0x100 of unk_02 tells whether button 100 is there.
 * func_ov006_023557b8 draws their icons at the same places. */
void Ov006_AddToolButtons(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  u8(*pos)[2];

  if (OV6->unitMode == 0) {
    pos = data_ov006_0236b6e8;
  } else {
    pos = data_ov006_0236b6f4;
  }
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(pos[0][0], pos[0][1], &gOv006ToolButtonTemplate, 96)->tile =
      0x24a;
  MenuButton_Add(pos[1][0], pos[1][1], &gOv006ToolButtonTemplate, 97)->tile =
      0x24e;
  MenuButton_Add(pos[2][0], pos[2][1], &gOv006ToolButtonTemplate, 98)->tile =
      0x256;
  MenuButton_Add(pos[3][0], pos[3][1], &gOv006ToolButtonTemplate, 99)->tile =
      0x252;
  if (OV6->unitMode != 0) {
    OV6->unk_02 &= ~0x100;
  } else {
    MenuButton_Add(pos[4][0], pos[4][1], &gOv006ToolButtonTemplate, 100)->tile =
        0x25a;
    OV6->unk_02 |= 0x100;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
