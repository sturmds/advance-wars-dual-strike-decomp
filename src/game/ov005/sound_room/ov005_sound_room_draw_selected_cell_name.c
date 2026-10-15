#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/text_print.h"
#include "sdk/msl.h"

const void* Text_Get(u32 textId);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);

extern const char data_ov005_0236b358[]; /* "%s" */

/* Clears the 22x2 tile text line at (5, 21) on layer 0 of the
 * data_027c0288.unk_04 screen and prints the name of the selected cell there,
 * if a row is selected. */
void Ov005SoundRoom_DrawSelectedCellName(Unk_ov005_0236bbac* work) {
  char text[45];
  Ov005CoCell* cells = work->cells;

  if (work->displayedRow == -1) {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Bg_FillRect(data_027c0280->layer0Map, 5, 21, 22, 2, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    snprintf(text, 45, data_ov005_0236b358,
             Text_Get(cells[work->displayedRow +
                            work->displayedPage * OV005_CELLS_PER_PAGE]
                          .name.id));
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Bg_FillRect(data_027c0280->layer0Map, 5, 21, 22, 2, 0);
    Text_Print(5, 21, data_027c0280->layer0Map, text, 0, 0x200);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
