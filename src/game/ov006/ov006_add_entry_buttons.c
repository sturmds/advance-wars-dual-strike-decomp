#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_button.h"
#include "game/unlocks.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236a7ec[][2]; /* (x, y) per entry when unitMode is 0 */
extern u8 data_ov006_0236a810[][2]; /* the same for unitMode 1, 25 entries */
extern u8 data_ov006_0236a844[][2]; /* the same, 26 entries (game flag 0x2b) */
extern const MenuButtonTemplate data_ov006_0236b854;

#define OV6 data_ov006_0236d640

/* Adds one menu button per entry of the list on the first screen, ids from
 * 64: 18 entries when unitMode is 0, else 25 or 26 by game flag 0x2b, at
 * the places Ov006_QueueEntrySprites draws them. unk_50 ends as the last
 * id. */
void Ov006_AddEntryButtons(void) {
  u8* pos;
  int dx;
  int dy;
  int i;
  int count = 0;
  int id = 64;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (OV6->unitMode == 1) {
    if (GameFlag_IsSet(0x2b) == 1) {
      pos = data_ov006_0236a844[0];
      count = 26;
    } else {
      pos = data_ov006_0236a810[0];
      count = 25;
    }
    dx = -4;
    dy = 0;
  }
  if (OV6->unitMode == 0) {
    pos = data_ov006_0236a7ec[0];
    count = 18;
    dx = -4;
    dy = 6;
  }
  OV6->unk_50 = 0;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < count; i++) {
    u8 x = *pos++;
    u8 y = *pos++;

    MenuButton_Add(x + dx, y + dy, &data_ov006_0236b854, id);
    OV6->unk_50 = id;
    id++;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
