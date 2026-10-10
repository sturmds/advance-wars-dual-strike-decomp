#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_ov006_0236d744[][30];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills data_ov006_0236d744 (30 bytes per row) with 1 on the map's border
 * tiles and 0 elsewhere. */
void Ov006_MarkMapBorder(void)
{
    int x;
    int y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            data_ov006_0236d744[y][x] = 0;
            if (x == 0) {
                data_ov006_0236d744[y][x] = 1;
            }
            if (y == 0) {
                data_ov006_0236d744[y][x] = 1;
            }
            if (x == STATE->unk_1c4->mapWidth - 1) {
                data_ov006_0236d744[y][x] = 1;
            }
            if (y == STATE->unk_1c4->mapHeight - 1) {
                data_ov006_0236d744[y][x] = 1;
            }
        }
    }
}
