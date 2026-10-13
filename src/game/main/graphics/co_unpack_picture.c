#include "types.h"
#include "game/owner_info.h"
#include "game/Unk_02088d04.h"

void Data_Unpack(const void* src, void* dst);

extern const void* const gCoPictureData[][136];
extern Unk_027c0280* data_027c0280;

/* Unpacks the compressed picture of a CO into OBJ VRAM, starting at tile
 * firstTile. The picture is picked by a byte in overlay 0 data. */
void Co_UnpackPicture(int coId, int firstTile) {
  Data_Unpack(gCoPictureData[coId & 0x7f][data_ov000_023503a0],
              data_027c0280->objVram + firstTile * 32);
}
