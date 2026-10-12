#include "types.h"

/* One text bank: ids from firstId upward index into strings (the low 24 bits). */
typedef struct TextBank {
  u32 firstId;
  const void** strings; /* 0 ends the list */
} TextBank;

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
