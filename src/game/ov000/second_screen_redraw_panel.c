#include "types.h"
#include "game/owner_info.h"
#include "game/second_screen.h"
#include "game/terrain.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Layers_ClearLeftBlock(void);
void Layer1_PutBlock(void);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, int offset, u32 size);
void Text_StopPrinting(void);
void SecondScreen_DrawTerrainDetails(int arg0, int arg1);
void SecondScreen_DrawTerrainMoveCosts(int arg0, int arg1);

extern void* data_ov000_022f65c0[];
extern const void* data_ov000_023501e8[][6];
extern const void* data_ov000_023501ec[][6];
extern const void* data_ov000_02350278[][6];
extern const void* data_ov000_0235027c[][6];

/* Second screen: clears the left block, then redraws the panel for the terrain in
 * data_ov000_02350530 (0: none, nothing is drawn). Terrain 8 and 6 take their picture
 * from per-owner tables indexed by data_ov000_0235053c; another terrain without a
 * picture of its own uses data_ov000_022f65c0[data_ov000_023503a0]. The picture goes
 * to OBJ VRAM, then either the name text goes to layer 0 (data_ov000_02350548 is 0)
 * or SecondScreen_DrawTerrainMoveCosts draws the numbers. The names are guesses. */
void SecondScreen_RedrawPanel(void) {
  const void* picture;
  const void* palette;

  Layers_ClearLeftBlock();
  if (data_ov000_02350530 == 0) {
    return;
  }
  Layer1_PutBlock();
  switch (data_ov000_02350530) {
    case 8:
      picture = data_ov000_023501e8[data_ov000_0235053c][0];
      palette = data_ov000_023501ec[data_ov000_0235053c][0];
      break;
    case 6:
      picture = data_ov000_02350278[data_ov000_0235053c][0];
      palette = data_ov000_0235027c[data_ov000_0235053c][0];
      break;
    default:
      picture = Ov000_gTerrainStats[data_ov000_02350530].picture;
      if (picture == NULL) {
        picture = data_ov000_022f65c0[data_ov000_023503a0];
      }
      palette = Ov000_gTerrainStats[data_ov000_02350530].palette;
      break;
  }
  Data_Unpack(picture, data_027c0280->objVram + 0x5280);
  Palette_Load(palette, 0x200, 0x20);
  /* needed to match: case 1 and default share one body, case 0 comes last */
  switch (data_ov000_02350548) {
    case 1:
    default:
      SecondScreen_DrawTerrainDetails(data_ov000_02350540, data_ov000_02350530);
      SecondScreen_DrawTerrainMoveCosts((u8)data_ov000_02350540,
                                        (u8)data_ov000_02350530);
      return;
    case 0:
      SecondScreen_DrawTerrainDetails(data_ov000_02350540, data_ov000_02350530);
      Text_StopPrinting();
      Text_PrintId(
          (short)(data_ov000_02350540 / 8 + 1), 11, data_027c0280->layer0Map,
          Ov000_gTerrainStats[data_ov000_02350530].description.id, 0, 256);
      return;
  }
}
