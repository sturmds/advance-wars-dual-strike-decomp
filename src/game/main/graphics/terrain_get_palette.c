#include "types.h"

extern s8 Ov000_gUrbanTerrain
    [32]; /* nonzero for urban terrain, indexed by terrain id */
extern u8 data_0214d19c[]; /* terrain palettes, 32 bytes each */
extern u8 data_02166759
    []; /* per terrain, 2 bytes each: the palette index is the second byte */

/* Palette of a terrain id. Urban terrain owned by an army (army 1 and up)
 * takes that army's palette, which follows the 6 shared ones. */
u8* Terrain_GetPalette(int terrain, int army) {
  if (Ov000_gUrbanTerrain[terrain & 0x1f] != 0 && army != 0) {
    return data_0214d19c + (army + 6) * 32;
  }
  return data_0214d19c + data_02166759[terrain * 2] * 32;
}
