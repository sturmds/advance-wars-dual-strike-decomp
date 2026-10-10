#include "types.h"

void func_ov006_02351828(int x, int y, int value);
int func_ov006_02351780(int x, int y, int value);
void func_ov006_0235dd24(int x, int y);

/* Sets the cell at x, y to value. If the tile above is plain, its cell is
 * reset to 1 (plain). Then refreshes the cell at x, y and the one to its
 * right (func_ov006_0235dd24). Painting a tile in the map editor is a
 * guess from this. */
void func_ov006_0235da1c(int x, int y, int value)
{
    func_ov006_02351828(x, y, value);
    if (func_ov006_02351780(x, y - 1, 1)) {
        func_ov006_02351828(x, y - 1, 1);
    }
    func_ov006_0235dd24(x, y);
}
