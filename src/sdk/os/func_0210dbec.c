#include "types.h"
#include "sdk/os.h"

#define SHARED_TYPE_BITS (*(vu16*)0x027ffffa)

#define TYPE_NOT_SET 0xffffffff
#define FLAG_10000000 0x10000000
#define FLAG_40000000 0x40000000
#define FLAG_20000000 0x20000000
#define FLAG_80000000 0x80000000
#define SLOT2_MATCH 0x01000000

static u32 data_0216d96c = {
#include "data/main/data_0216d96c.inc.c"
};
/* ":/" */
char gArchiveNameColonSlash[4] = {
#include "data/main/gArchiveNameColonSlash.inc.c"
};

u32 func_0210db18(void);
BOOL func_0210dae4(void);
BOOL func_0210dc64(void);

/* Computed on the first call, then served from data_0216d96c. */
u32 func_0210dbec(void) {
  if (data_0216d96c == TYPE_NOT_SET) {
    u32 slot2 = func_0210db18();
    u32 type;

    if (func_0210dc64()) {
      type = slot2 | FLAG_10000000;
    } else if (func_0210dae4()) {
      type = slot2 | FLAG_40000000;
    } else if (slot2 & SLOT2_MATCH) {
      type = slot2 | FLAG_20000000;
    } else {
      type = slot2 | FLAG_80000000;
    }
    data_0216d96c = type | SHARED_TYPE_BITS;
  }
  return data_0216d96c;
}
