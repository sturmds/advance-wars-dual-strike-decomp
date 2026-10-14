#include "types.h"
#include "game/text_ref.h"

extern TextBank gTextBanks[];

const void* Text_Get(u32 textId) {
  TextBank* bank = gTextBanks;

  while (bank->strings != 0) {
    if (textId >= bank->firstId) {
      return bank->strings[textId & 0xffffff];
    }
    bank++;
  }
  return 0;
}
