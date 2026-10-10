#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"

extern s16 data_021389e8[];
extern s16 data_02138ae8[];
extern s16 data_02138be8[];
extern Ov003TileInfo data_ov003_02364d88[];

Ov003Unit *Ov003Marker_FindNearestUnit(Unk_ov003_02369bb8 *marker);
void Ov003Object_SpawnEffect(Unk_ov003_02369bb8 *marker, int velX, int velY);

/* Shoots at the nearest unit: one effect along the vector to it, then one
 * along each of two vectors fanned out to either side (v plus v rotated by a
 * fixed angle, using the sine table), each only if it points to the side of
 * the marker that unk_08_4 allows (0 down, 1 up, 2 left, 3 right). Then unk_0b waits Ov003TileInfo.unk_06 frames. Needed to match: case
 * 1 before case 0. */
void Ov003Marker_FireAtNearestUnit(Unk_ov003_02369bb8 *marker)
{
    Ov003Unit *target = Ov003Marker_FindNearestUnit(marker);
    int dx;
    int dy;
    int leftX;
    int leftY;
    int rightX;
    int rightY;

    if (target == NULL) {
        return;
    }
    dx = (target->x >> 3) - (((marker->x << 7) + 64) >> 3);
    dy = (target->y >> 3) - (((marker->y << 7) + 64) >> 3);
    Ov003Object_SpawnEffect(marker, dx, dy);
    leftX = dx + ((data_021389e8[85] * dx) >> 12) - ((data_021389e8[21] * dy) >> 12);
    leftY = dy + ((data_021389e8[21] * dx) >> 12) + ((data_021389e8[85] * dy) >> 12);
    rightX = dx + ((data_02138be8[43] * dx) >> 12) - ((data_02138ae8[107] * dy) >> 12);
    rightY = dy + ((data_02138ae8[107] * dx) >> 12) + ((data_02138be8[43] * dy) >> 12);
    switch (marker->unk_08_4) {
    case 1:
        if (leftY <= 0) {
            Ov003Object_SpawnEffect(marker, leftX, leftY);
        }
        if (rightY <= 0) {
            Ov003Object_SpawnEffect(marker, rightX, rightY);
        }
        break;
    case 0:
        if (leftY >= 0) {
            Ov003Object_SpawnEffect(marker, leftX, leftY);
        }
        if (rightY >= 0) {
            Ov003Object_SpawnEffect(marker, rightX, rightY);
        }
        break;
    case 2:
        if (leftX <= 0) {
            Ov003Object_SpawnEffect(marker, leftX, leftY);
        }
        if (rightX <= 0) {
            Ov003Object_SpawnEffect(marker, rightX, rightY);
        }
        break;
    case 3:
        if (leftX >= 0) {
            Ov003Object_SpawnEffect(marker, leftX, leftY);
        }
        if (rightX >= 0) {
            Ov003Object_SpawnEffect(marker, rightX, rightY);
        }
        break;
    }
    marker->unk_0b = data_ov003_02364d88[marker->unk_08_0].unk_06;
}
