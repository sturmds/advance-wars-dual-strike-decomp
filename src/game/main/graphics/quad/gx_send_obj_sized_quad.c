#include "types.h"

void Gx_SendPlainQuad_0200fc10(void);
void Gx_SendPlainQuad_0200fcfc(void);
void Gx_SendPlainQuad_0200fde8(void);
void Gx_SendPlainQuad_0200fed4(void);
void Gx_SendPlainQuad_0200ffc0(void);
void Gx_SendPlainQuad_020100ac(void);
void Gx_SendPlainQuad_02010198(void);
void Gx_SendPlainQuad_02010284(void);
void Gx_SendPlainQuad_02010370(void);
void Gx_SendPlainQuad_0201045c(void);
void Gx_SendPlainQuad_02010548(void);
void Gx_SendPlainQuad_02010634(void);
void Gx_SendPlainQuad_02010720(void);
void Gx_SendPlainQuad_0201080c(void);
void Gx_SendPlainQuad_020108f8(void);
void Gx_SendPlainQuad_020109e4(void);
void Gx_SendPlainQuad_02010ad0(void);
void Gx_SendPlainQuad_02010bbc(void);
void Gx_SendPlainQuad_02010ca8(void);
void Gx_SendPlainQuad_02010d94(void);
void Gx_SendPlainQuad_02010e80(void);
void Gx_SendPlainQuad_02010f6c(void);
void Gx_SendPlainQuad_02011058(void);
void Gx_SendPlainQuad_02011144(void);
void Gx_SendPlainQuad_02011230(void);
void Gx_SendPlainQuad_0201131c(void);
void Gx_SendPlainQuad_02011408(void);
void Gx_SendPlainQuad_020114f4(void);
void Gx_SendPlainQuad_020115e0(void);
void Gx_SendPlainQuad_020116cc(void);
void Gx_SendPlainQuad_020117b8(void);
void Gx_SendPlainQuad_020118a4(void);
void Gx_SendPlainQuad_02011990(void);
void Gx_SendPlainQuad_02011a7c(void);
void Gx_SendPlainQuad_02011b68(void);
void Gx_SendPlainQuad_02011c54(void);
void Gx_SendPlainQuad_02011d40(void);
void Gx_SendPlainQuad_02011e2c(void);
void Gx_SendPlainQuad_02011f18(void);
void Gx_SendPlainQuad_02012004(void);
void Gx_SendPlainQuad_020120f0(void);
void Gx_SendPlainQuad_020121dc(void);
void Gx_SendPlainQuad_020122c8(void);
void Gx_SendPlainQuad_020123b4(void);

/* Flip bits as in OAM attribute 1 (guess from the values). */
#define QUAD_FLIP_H 0x1000
#define QUAD_FLIP_V 0x2000

/* Sends the textured quad for an OBJ-sized image: width and height in
 * pixels (the twelve DS OBJ sizes) and the flip bits pick one of the
 * Gx_SendPlainQuad_* functions (64x32 and 64x64 share one). Other sizes send nothing. A height not
 * listed under its width falls through to the next width's cases, as in
 * the original. */
void Gx_SendObjSizedQuad(int width, int height, int flip) {
  switch (flip) {
    case QUAD_FLIP_H:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02011990();
              return;
            case 16:
              Gx_SendPlainQuad_0201131c();
              return;
            case 32:
              Gx_SendPlainQuad_02011230();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_020115e0();
              return;
            case 16:
              Gx_SendPlainQuad_020118a4();
              return;
            case 32:
              Gx_SendPlainQuad_02011144();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_020114f4();
              return;
            case 16:
              Gx_SendPlainQuad_02011408();
              return;
            case 32:
              Gx_SendPlainQuad_020117b8();
              return;
            case 64:
              Gx_SendPlainQuad_02011058();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_020116cc();
              return;
            case 64:
              Gx_SendPlainQuad_020116cc();
              return;
          }
      }
      break;
    case QUAD_FLIP_V:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02010f6c();
              return;
            case 16:
              Gx_SendPlainQuad_020108f8();
              return;
            case 32:
              Gx_SendPlainQuad_0201080c();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02010bbc();
              return;
            case 16:
              Gx_SendPlainQuad_02010e80();
              return;
            case 32:
              Gx_SendPlainQuad_02010720();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02010ad0();
              return;
            case 16:
              Gx_SendPlainQuad_020109e4();
              return;
            case 32:
              Gx_SendPlainQuad_02010d94();
              return;
            case 64:
              Gx_SendPlainQuad_02010634();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_02010ca8();
              return;
            case 64:
              Gx_SendPlainQuad_02010ca8();
              return;
          }
      }
      break;
    case QUAD_FLIP_H | QUAD_FLIP_V:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02010548();
              return;
            case 16:
              Gx_SendPlainQuad_0200fed4();
              return;
            case 32:
              Gx_SendPlainQuad_0200fde8();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02010198();
              return;
            case 16:
              Gx_SendPlainQuad_0201045c();
              return;
            case 32:
              Gx_SendPlainQuad_0200fcfc();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_020100ac();
              return;
            case 16:
              Gx_SendPlainQuad_0200ffc0();
              return;
            case 32:
              Gx_SendPlainQuad_02010370();
              return;
            case 64:
              Gx_SendPlainQuad_0200fc10();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_02010284();
              return;
            case 64:
              Gx_SendPlainQuad_02010284();
              return;
          }
      }
      break;
    default:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_020123b4();
              return;
            case 16:
              Gx_SendPlainQuad_02011d40();
              return;
            case 32:
              Gx_SendPlainQuad_02011c54();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02012004();
              return;
            case 16:
              Gx_SendPlainQuad_020122c8();
              return;
            case 32:
              Gx_SendPlainQuad_02011b68();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_02011f18();
              return;
            case 16:
              Gx_SendPlainQuad_02011e2c();
              return;
            case 32:
              Gx_SendPlainQuad_020121dc();
              return;
            case 64:
              Gx_SendPlainQuad_02011a7c();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_020120f0();
              return;
            case 64:
              Gx_SendPlainQuad_020120f0();
              return;
          }
      }
      break;
  }
}
