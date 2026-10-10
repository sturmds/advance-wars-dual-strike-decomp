#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"

extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Four bytes per panel page, passed on to func_020f75dc. */
extern u8 data_ov000_0234f9e8[][4];

const void *Text_Get(u32 textId);
int func_02014a64(const void *text);
void Screen_SetCurrentGfx(void *state);
void func_020f75dc(int a, int b, int c, int d, int e, int f);

/* Draws the current page's marker on the second screen. Pages 4 and 11 pass
 * the pixel width of the unit's second and third description text instead of
 * the table's third byte. */
void UnitInfoPanel_DrawPageMarker(void)
{
    int page = data_ov000_0234ec8c->page;
    int base = data_ov000_0234f9e8[page][0];
    int x = data_ov000_0234f9e8[page][1];
    int value = data_ov000_0234f9e8[page][2];
    int d = data_ov000_0234f9e8[page][3];
    void *savedGfx;

    switch (page) {
    case 4:
        value = func_02014a64(Text_Get(Ov000_gUnitStats[data_ov000_0234ec8c->unit->type].text[1].id));
        break;
    case 11:
        value = func_02014a64(Text_Get(Ov000_gUnitStats[data_ov000_0234ec8c->unit->type].text[2].id));
        break;
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    func_020f75dc(data_ov000_0234ec8c->unk_00 + base, x, value, d, 952, 15);
    Screen_SetCurrentGfx(savedGfx);
}
