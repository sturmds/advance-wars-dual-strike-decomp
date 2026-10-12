#include "types.h"

void Gx_SendPlainQuad_0200cfd0(void);
void Gx_SendPlainQuad_0200d0bc(void);
void Gx_SendPlainQuad_0200d1a8(void);
void Gx_SendPlainQuad_0200d294(void);
void Gx_SendPlainQuad_0200d380(void);
void Gx_SendPlainQuad_0200d46c(void);
void Gx_SendPlainQuad_0200d558(void);
void Gx_SendPlainQuad_0200d644(void);
void Gx_SendPlainQuad_0200d730(void);
void Gx_SendPlainQuad_0200d81c(void);
void Gx_SendPlainQuad_0200d908(void);
void Gx_SendPlainQuad_0200d9f4(void);
void Gx_SendPlainQuad_0200dae0(void);
void Gx_SendPlainQuad_0200dbcc(void);
void Gx_SendPlainQuad_0200dcb8(void);
void Gx_SendPlainQuad_0200dda4(void);
void Gx_SendPlainQuad_0200de90(void);
void Gx_SendPlainQuad_0200df7c(void);
void Gx_SendPlainQuad_0200e068(void);
void Gx_SendPlainQuad_0200e154(void);
void Gx_SendPlainQuad_0200e240(void);
void Gx_SendPlainQuad_0200e32c(void);
void Gx_SendPlainQuad_0200e418(void);
void Gx_SendPlainQuad_0200e504(void);
void Gx_SendPlainQuad_0200e5f0(void);
void Gx_SendPlainQuad_0200e6dc(void);
void Gx_SendPlainQuad_0200e7c8(void);
void Gx_SendPlainQuad_0200e8b4(void);
void Gx_SendPlainQuad_0200e9a0(void);
void Gx_SendPlainQuad_0200ea8c(void);
void Gx_SendPlainQuad_0200eb78(void);
void Gx_SendPlainQuad_0200ec64(void);
void Gx_SendPlainQuad_0200ed50(void);
void Gx_SendPlainQuad_0200ee3c(void);
void Gx_SendPlainQuad_0200ef28(void);
void Gx_SendPlainQuad_0200f014(void);
void Gx_SendPlainQuad_0200f100(void);
void Gx_SendPlainQuad_0200f1ec(void);
void Gx_SendPlainQuad_0200f2d8(void);
void Gx_SendPlainQuad_0200f3c4(void);
void Gx_SendPlainQuad_0200f4b0(void);
void Gx_SendPlainQuad_0200f59c(void);
void Gx_SendPlainQuad_0200f688(void);
void Gx_SendPlainQuad_0200f774(void);
void Gx_SendPlainQuad_0200f860(void);
void Gx_SendPlainQuad_0200f94c(void);
void Gx_SendPlainQuad_0200fa38(void);
void Gx_SendPlainQuad_0200fb24(void);

#define QUAD_FLIP_H 0x1000
#define QUAD_FLIP_V 0x2000

/* Gx_SendObjSizedQuad with another set of quads; here 64x32 and 64x64
 * get different ones. SpriteCells_Draw3d calls this one when its flag is set. */
void func_0201403c(int width, int height, int flip) {
  switch (flip) {
    case QUAD_FLIP_H:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200f014();
              return;
            case 16:
              Gx_SendPlainQuad_0200e8b4();
              return;
            case 32:
              Gx_SendPlainQuad_0200e7c8();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200ec64();
              return;
            case 16:
              Gx_SendPlainQuad_0200ef28();
              return;
            case 32:
              Gx_SendPlainQuad_0200e6dc();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200eb78();
              return;
            case 16:
              Gx_SendPlainQuad_0200ea8c();
              return;
            case 32:
              Gx_SendPlainQuad_0200ee3c();
              return;
            case 64:
              Gx_SendPlainQuad_0200e5f0();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_0200e9a0();
              return;
            case 64:
              Gx_SendPlainQuad_0200ed50();
              return;
          }
      }
      break;
    case QUAD_FLIP_V:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200e504();
              return;
            case 16:
              Gx_SendPlainQuad_0200dda4();
              return;
            case 32:
              Gx_SendPlainQuad_0200dcb8();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200e154();
              return;
            case 16:
              Gx_SendPlainQuad_0200e418();
              return;
            case 32:
              Gx_SendPlainQuad_0200dbcc();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200e068();
              return;
            case 16:
              Gx_SendPlainQuad_0200df7c();
              return;
            case 32:
              Gx_SendPlainQuad_0200e32c();
              return;
            case 64:
              Gx_SendPlainQuad_0200dae0();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_0200de90();
              return;
            case 64:
              Gx_SendPlainQuad_0200e240();
              return;
          }
      }
      break;
    case QUAD_FLIP_H | QUAD_FLIP_V:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200d9f4();
              return;
            case 16:
              Gx_SendPlainQuad_0200d294();
              return;
            case 32:
              Gx_SendPlainQuad_0200d1a8();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200d644();
              return;
            case 16:
              Gx_SendPlainQuad_0200d908();
              return;
            case 32:
              Gx_SendPlainQuad_0200d0bc();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200d558();
              return;
            case 16:
              Gx_SendPlainQuad_0200d46c();
              return;
            case 32:
              Gx_SendPlainQuad_0200d81c();
              return;
            case 64:
              Gx_SendPlainQuad_0200cfd0();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_0200d380();
              return;
            case 64:
              Gx_SendPlainQuad_0200d730();
              return;
          }
      }
      break;
    default:
      switch (width) {
        case 8:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200fb24();
              return;
            case 16:
              Gx_SendPlainQuad_0200f3c4();
              return;
            case 32:
              Gx_SendPlainQuad_0200f2d8();
              return;
          }
        case 16:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200f774();
              return;
            case 16:
              Gx_SendPlainQuad_0200fa38();
              return;
            case 32:
              Gx_SendPlainQuad_0200f1ec();
              return;
          }
        case 32:
          switch (height) {
            case 8:
              Gx_SendPlainQuad_0200f688();
              return;
            case 16:
              Gx_SendPlainQuad_0200f59c();
              return;
            case 32:
              Gx_SendPlainQuad_0200f94c();
              return;
            case 64:
              Gx_SendPlainQuad_0200f100();
              return;
          }
        case 64:
          switch (height) {
            case 32:
              Gx_SendPlainQuad_0200f4b0();
              return;
            case 64:
              Gx_SendPlainQuad_0200f860();
              return;
          }
      }
      break;
  }
}
