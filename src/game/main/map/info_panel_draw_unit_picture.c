#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/info_panel.h"

extern s16 data_0216635e[INFO_PANEL_SIDES][3]; /* unit box x */
extern u8 gInfoPanelUnitPictureShape[];        /* sprite shape of the picture */

u8* UnitType_GetPictureTiles(int unitType);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Unit box of the info panel: queues the 8 picture tiles that
 * UnitType_GetPictureTiles gives for the unit type (the caller passes
 * MapUnit.type) and draws them as a sprite in palette 1. */
void InfoPanel_DrawUnitPicture(int type, int panelX, int panelY, int side) {
  TransferQueue_Add(UnitType_GetPictureTiles(type),
                    data_027c0280->objVram + 0x6800, 0x100);
  Oam_PutSprite(
      (data_021663f4.unk_02.x + (panelX + data_0216635e[side][0])) & 0x1ff,
      (panelY + data_021663f4.unk_02.y) | 0x400, gInfoPanelUnitPictureShape,
      1 << 12);
}
