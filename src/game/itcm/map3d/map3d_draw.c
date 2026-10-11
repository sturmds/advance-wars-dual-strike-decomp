#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern volatile s16 data_0216e2ac;
extern volatile s16 data_0216e2a4;
extern u8 data_ov000_022dbd41[]; /* Ov000_gMapHeaders + 0x19, 0xa0 bytes per map */

void Map3d_DrawTiles(VecFx32 *shake, int arg);
int func_01ffd854(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Draws the 3D map on the screen data_027c0288.unk_00 shows, moved by the
 * screen shake offsets. Does nothing for the other screen's state. */
void Map3d_Draw(void)
{
    if ((void *)data_027c027c == data_027c0288.unk_00) {
        VecFx32 shake = {0, 0, 0};

        shake.x = data_0216e2ac;
        shake.y = data_0216e2a4;
        Map3d_DrawTiles(&shake, data_ov000_022dbd41[STATE->unk_1c4->mapId * sizeof(MapHeader)]);
        func_01ffd854();
    }
}
