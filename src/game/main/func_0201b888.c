#include "types.h"

typedef struct Unk_0201b888_0 Unk_0201b888_0;
typedef struct Unk_0201b888 Unk_0201b888;

struct Unk_0201b888_0 {
  u8 pad_00[0x8];
  s16 unk_8;
  s16 unk_a;
  int unk_c;
};

struct Unk_0201b888 {
  u8 pad_00[0x14];
  Unk_0201b888_0* unk_14;
};

void MapCursor_DrawAt(int, int, int);

void func_0201b888(Unk_0201b888* a0) {
  Unk_0201b888_0* t0 = a0->unk_14;
  MapCursor_DrawAt(t0->unk_8, t0->unk_a, t0->unk_c);
}
