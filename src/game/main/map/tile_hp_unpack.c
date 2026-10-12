#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Reads the data TileHp_Pack wrote back into the current state's tile hp. */
void TileHp_Unpack(u8* data) {
  int i;
  u8* hp;
  int code;

  hp = STATE->tileHp;
  code = *data;

  if (code == 0xff) {
    return;
  }
  do {
    if (code & 0x80) {
      *hp++ = *data++ & 0x7f;
    } else {
      for (i = 0; i < *data; i++) {
        *hp++ = 0;
      }
      data++;
    }
    code = *data;
  } while (code != 0xff);
}
