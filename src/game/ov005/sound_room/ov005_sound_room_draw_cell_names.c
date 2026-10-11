#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/text_print.h"

const void *Text_Get(u32 textId);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
int func_02122414(char *buf, int size, const char *format, ...); /* snprintf */

extern const char data_ov005_0236b358[]; /* "%s" */

/* Clears the eight two-tile text rows from (5, 7) on layer 0 of the
 * data_027c0288.unk_00 screen and prints the names of the current page's
 * cells into them. */
void Ov005SoundRoom_DrawCellNames(Unk_ov005_0236bbac *work)
{
    char text[45];
    int first;
    int rows;
    int i;

    first = work->page * OV005_CELLS_PER_PAGE;
    rows = OV005_CO_CELL_COUNT - first;
    if (rows > OV005_CELLS_PER_PAGE) {
        rows = OV005_CELLS_PER_PAGE;
    }
    for (i = 0; i < OV005_CELLS_PER_PAGE; i++) {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;

        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Bg_FillRect(data_027c0280->layer0Map, 5, 7 + i * 2, 22, 2, 0);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    for (i = 0; i < rows; i++) {
        Unk_027c0280 *saved0280;
        Unk_02080440 *saved027c;

        func_02122414(text, 45, data_ov005_0236b358, Text_Get(work->cells[i + first].name.id));
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Text_Print(5, 7 + i * 2, data_027c0280->layer0Map, text, 0, 0x200 + i * 44);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
}
