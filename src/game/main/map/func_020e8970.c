#include "types.h"
#include "game/Unk_02080440.h"

void func_020b9864(void);
void func_020b9890(void);
void Map_InitBattle(void);
BOOL func_020e7f30(int mapId);
void Map_SetupPlayerCos(void);
void func_020e88f8(void);
void Map_SetCoSelectPhase(u8 value);

/* Sets up the players from the map header, then, if func_020e7f30 holds for
 * the current map, sets coSelectPhase to 1 and runs more setup. func_020b9890 and
 * func_020b9864 set Unk_02080440_48 byte 0x0d to 1 and 0. */
void func_020e8970(void) {
  Map_SetupPlayerCos();
  if (func_020e7f30(data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId)) {
    Map_SetCoSelectPhase(1);
    Map_InitBattle();
    func_020e88f8();
    func_020b9890();
  } else {
    func_020b9864();
  }
}
