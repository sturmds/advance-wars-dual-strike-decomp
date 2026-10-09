#include "types.h"
#include "game/menu_button.h"

/* Moves the menu cursor corners to a rectangle at once; MenuCursor_Update
 * then eases them from there. */
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom)
{
    data_02158f38->corners[0][0] = left;
    data_02158f38->corners[0][1] = top;
    data_02158f38->corners[1][0] = right;
    data_02158f38->corners[1][1] = top;
    data_02158f38->corners[2][0] = left;
    data_02158f38->corners[2][1] = bottom;
    data_02158f38->corners[3][0] = right;
    data_02158f38->corners[3][1] = bottom;
}
