#include "types.h"

int Ov006_GetNeighborTile(int x, int y);
int func_ov006_02360f90(int x, int y);
int func_ov006_02360e10(int x, int y);

/* Combines three lookups for the tile at (x, y). When Ov006_GetNeighborTile
 * returns 0, the result is func_ov006_02360f90's value, replaced by
 * func_ov006_02360e10's when both are nonzero. Otherwise the result is
 * Ov006_GetNeighborTile's value, replaced by func_ov006_02360e10's when that is
 * nonzero and bits 9-15 of the first value are 0, 0x2000 or 0x4000. The
 * repeated group tests are needed to match. */
int func_ov006_023624b4(int x, int y)
{
    int result = Ov006_GetNeighborTile(x, y);
    int other;
    int group;

    if (result == 0) {
        result = func_ov006_02360f90(x, y);
        if (result != 0) {
            other = func_ov006_02360e10(x, y);
            if (other != 0) {
                result = other;
            }
        }
    } else {
        other = func_ov006_02360e10(x, y);
        if (other != 0) {
            group = result & 0xfe00;
            if (group == 0x4000) {
                if ((result & 0xfe00) == 0x4000) {
                    result = other;
                }
            }
            if (group == 0x2000) {
                if ((result & 0xfe00) == 0x2000) {
                    result = other;
                }
            }
            if (group == 0) {
                result = other;
            }
        }
    }
    return result;
}
