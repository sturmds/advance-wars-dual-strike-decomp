#include "types.h"
#include "game/tab_list.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void TabList_PutIconQuad(int x, int y, int index, int player, int palette);
void Number_Draw(s16 x, s16 y, u16 *map, int value, u16 a, u16 b);
extern Unk_027c0280 *data_027c0280;

/* TabListEntry.draw of the entries with an Orange Star icon: like
 * TabList_DrawEntryWithIcon, but icon unk_04 comes from army style 1 (Orange
 * Star; TabList_DrawEntryWithIcon uses 5, Black Hole) with palette 3. */
void TabList_DrawEntryWithOrangeStarIcon(int x, int y, int row, int col)
{
    u16 *map;

    TabList_PutIconQuad(x, y, data_0215d6c4[row][col].unk_04, 1, 3);
    Text_PrintId(x + 2, y, data_027c0280->layer0Map, data_0215d6c4[row][col].textId, 0, ((y - 5) >> 1) * 64 + 128);
    map = data_027c0280->layer0Map;
    Number_Draw(x + 20, y, map, data_0215d6c4[row][col].getValue(row, col), 0, 0);
}
