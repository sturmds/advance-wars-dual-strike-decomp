#include "types.h"

/* Cell tables in DTCM, one per OAM shape and size. */
extern u8 data_027c0000;
extern u8 data_027c0008;
extern u8 data_027c0010;
extern u8 data_027c0018;
extern u8 data_027c0020;
extern u8 data_027c0028;
extern u8 data_027c0030;
extern u8 data_027c0038;
extern u8 data_027c0040;
extern u8 data_027c0048;
extern u8 data_027c0058;
extern u8 data_027c0060;

/* Returns the cell table for a sprite of the given pixel width and height,
 * or NULL when no OAM size has that shape. Oam_GetAttrCells picks from the
 * same tables by OAM attributes; 8x32 uses data_027c0010, which it never
 * returns. */
const void* Oam_GetShapeCells(int width, int height) {
  switch (width) {
    case 8:
      switch (height) {
        case 8:
          return &data_027c0028;
        case 16:
          return &data_027c0038;
        case 32:
          return &data_027c0010;
      }
      return NULL;
    case 16:
      switch (height) {
        case 8:
          return &data_027c0058;
        case 16:
          return &data_027c0040;
        case 32:
          return &data_027c0000;
      }
      return NULL;
    case 32:
      switch (height) {
        case 8:
          return &data_027c0020;
        case 16:
          return &data_027c0060;
        case 32:
          return &data_027c0048;
        case 64:
          return &data_027c0018;
      }
      return NULL;
    case 64:
      switch (height) {
        case 32:
          return &data_027c0008;
        case 64:
          return &data_027c0030;
      }
      return NULL;
  }
  return NULL;
}
