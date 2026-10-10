#include "types.h"
#include "game/Unk_ov003_02369bb8.h"

void Ov003Marker_FireAtNearestUnit(Unk_ov003_02369bb8 *entry);
void Ov003Marker_FireBurst(Unk_ov003_02369bb8 *entry);

/* Runs the entry's action (kind 7 has its own) once both countdowns have reached
 * 0, then steps the countdowns and unk_09_6 down. */
void Ov003_StepEntry(Unk_ov003_02369bb8 *entry)
{
    if (entry->unk_0a == 0 && entry->unk_0b == 0) {
        if (entry->unk_08_0 == 7) {
            Ov003Marker_FireBurst(entry);
        } else {
            Ov003Marker_FireAtNearestUnit(entry);
        }
    }
    if (entry->unk_0a != 0) {
        entry->unk_0a--;
    }
    if (entry->unk_0b != 0) {
        entry->unk_0b--;
    }
    if (entry->unk_09_6 != 0) {
        entry->unk_09_6--;
    }
}
