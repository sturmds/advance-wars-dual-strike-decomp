#include "types.h"
#include "game/co_graphics.h"
#include "game/Unk_02088d04.h"

void func_02112658(const char *dir);                 /* changes the current file system directory */
int func_02002c04(int coId);                         /* picture variant stored for this CO */
void func_0200bcf0(void *dest, const char *fileName); /* loads a file into dest */

extern char data_02151e44[];                         /* "/syogun" */
extern Unk_027c0280 *data_027c0280;

/* Loads the picture of a CO from /syogun into the tiles starting at
 * firstTile. */
void Co_LoadGraphics(int coId, int firstTile)
{
    int variant;

    func_02112658(data_02151e44);
    variant = func_02002c04(coId);
    func_0200bcf0(data_027c0280->charBase + firstTile * 32,
                  data_02152b38[coId & 0x7f].pictures[variant].name);
}
