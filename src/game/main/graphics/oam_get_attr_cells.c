#include "types.h"
#include "game/oam_cells.h"

#define OBJ_SHAPE_SQUARE 0x0000
#define OBJ_SHAPE_WIDE 0x4000
#define OBJ_SHAPE_TALL 0x8000

/* Returns the cell table for the shape (attribute 0 bits 14-15) and size
 * (attribute 1 bits 14-15) of the OAM attributes attr, or NULL for the
 * unused shape. Oam_GetShapeCells picks from the same tables by width and
 * height. Tall sizes 0 and 1 both return data_027c0038. */
const void* Oam_GetAttrCells(const u16* attr) {
  u16 size = attr[1] & 0xc000;
  u16 shape = attr[0] & 0xc000;

  switch (shape) {
    case OBJ_SHAPE_SQUARE:
      switch (size) {
        case 0x0000:
          return &data_027c0028;
        case 0x4000:
          return &data_027c0040;
        case 0x8000:
          return &data_027c0048;
        case 0xc000:
          return &data_027c0030;
      }
      break;
    case OBJ_SHAPE_WIDE:
      switch (size) {
        case 0x0000:
          return &data_027c0058;
        case 0x4000:
          return &data_027c0020;
        case 0x8000:
          return &data_027c0060;
        case 0xc000:
          return &data_027c0008;
      }
      break;
    case OBJ_SHAPE_TALL:
      switch (size) {
        case 0x0000:
          return &data_027c0038;
        case 0x4000:
          return &data_027c0038;
        case 0x8000:
          return &data_027c0000;
        case 0xc000:
          return &data_027c0018;
      }
      break;
  }
  return NULL;
}
