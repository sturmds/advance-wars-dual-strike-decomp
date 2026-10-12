#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern u8 data_ov006_0236d568[2][16];
extern u8 data_ov006_0236d588[2][16];
extern u8 data_ov006_0236d5a8[2][16];
extern u8 data_ov006_0236d5c8[2][16];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Index of the first of 16 rows whose four tables (one per player 1-4) hold,
 * halved, the army colours of the current map state; 0 when none does. Each
 * table is two rows of 16 bytes and the second row is read. */
int Ov006_FindArmyLayout(void) {
  int result = 0;
  int i;

  for (i = 0; i < 16; i++) {
    if (data_ov006_0236d568[1][i] >> 1 == STATE->army[1] &&
        data_ov006_0236d588[1][i] >> 1 == STATE->army[2] &&
        data_ov006_0236d5a8[1][i] >> 1 == STATE->army[3] &&
        data_ov006_0236d5c8[1][i] >> 1 == STATE->army[4]) {
      result = i;
      break;
    }
  }
  return result;
}
