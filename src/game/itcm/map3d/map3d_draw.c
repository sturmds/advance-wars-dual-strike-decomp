#include "types.h"
#include "game/screen_shake.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"

extern u8
    data_ov000_022dbd41[]; /* Ov000_gMapHeaders + 0x19, 0xa0 bytes per map */

void Map3d_DrawTiles(VecFx32* shake, int arg);
int func_01ffd854(void);

/* Draws the 3D map on the screen data_027c0288.unk_00 shows, moved by the
 * screen shake offsets. Does nothing for the other screen's state. */
void Map3d_Draw(void) {
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    VecFx32 shake = {0, 0, 0};

    shake.x = gScreenShakeX;
    shake.y = gScreenShakeY;
    Map3d_DrawTiles(
        &shake,
        data_ov000_022dbd41[MAP_STATE->unk_1c4->mapId * sizeof(MapHeader)]);
    func_01ffd854();
  }
}
