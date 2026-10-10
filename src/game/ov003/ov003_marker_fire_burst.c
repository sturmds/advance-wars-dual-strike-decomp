#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"

extern Ov003TileInfo data_ov003_02364d88[];

Ov003Unit *Ov003Marker_FindNearestUnit(Unk_ov003_02369bb8 *marker);
void Ov003Object_SpawnEffect(Unk_ov003_02369bb8 *marker, int velX, int velY);
int func_020279c4(int x1, int y1, int x2, int y2);
void Ov003_StepToward(int x1, int y1, int x2, int y2, int length, int *outX, int *outY);

/* Fires one effect per frame in a burst of Ov003TileInfo.unk_05 frames
 * (unk_09_1 counts them). The first frame picks the nearest unit as the
 * target and aims at it; later frames aim at the same unit, the vector cut to
 * Ov003TileInfo.unk_04 tiles. The burst ends, and unk_0b starts its wait of
 * unk_06 frames, when the count is reached or the target is gone. Needed to
 * match: the declaration order. */
void Ov003Marker_FireBurst(Unk_ov003_02369bb8 *marker)
{
    int shotX;
    Ov003Unit *target;
    int shotY;
    int dx;
    int reach;
    int dy;

    if (marker->unk_09_1 != 0) {
        if (marker->target == NULL) {
            marker->unk_09_1 = 0;
            marker->unk_0b = data_ov003_02364d88[marker->unk_08_0].unk_06;
            return;
        }
        reach = (data_ov003_02364d88[marker->unk_08_0].unk_04 << 7) >> 3;
        dx = (marker->target->x >> 3) - (((marker->x << 7) + 64) >> 3);
        dy = (marker->target->y >> 3) - (((marker->y << 7) + 64) >> 3);
        if (reach < func_020279c4(dx, dy, 0, 0)) {
            Ov003_StepToward(0, 0, dx, dy, reach, &shotX, &shotY);
            dx = shotX;
            dy = shotY;
        }
    } else {
        target = Ov003Marker_FindNearestUnit(marker);
        if (target == NULL) {
            return;
        }
        dx = (target->x >> 3) - (((marker->x << 7) + 64) >> 3);
        dy = (target->y >> 3) - (((marker->y << 7) + 64) >> 3);
        marker->target = target;
    }
    Ov003Object_SpawnEffect(marker, dx, dy);
    marker->unk_09_1++;
    if (marker->unk_09_1 != data_ov003_02364d88[marker->unk_08_0].unk_05) {
        return;
    }
    marker->unk_09_1 = 0;
    marker->unk_0b = data_ov003_02364d88[marker->unk_08_0].unk_06;
}
