#include "types.h"
#include "sdk/msl_wchar.h"

extern void* data_0216dd84[];
extern char data_0216dda4[];
extern u32 data_0216ddfc[];
void func_02122348(void);
extern u32 data_0216dd74[];
extern char data_0216de70[];

#pragma explicit_zero_data on
u32 data_0216dd2c = {
#include "data/main/data_0216dd2c.inc.c"
};
u32 data_0216dd28 = {
#include "data/main/data_0216dd28.inc.c"
};
char data_0216dd30[6] = {
#include "data/main/data_0216dd30.inc.c"
};
void* data_0216dd38[2] = {&__mbtowc_noconv, &func_02122348};
char data_0216dd40[9] = {
#include "data/main/data_0216dd40.inc.c"
};
char data_0216dd58[12] = {
#include "data/main/data_0216dd58.inc.c"
};
MslLocale gMslLocale = {
#include "data/main/gMslLocale.inc.c"
};
char data_0216dd64[15] = {
#include "data/main/data_0216dd64.inc.c"
};
u32 data_0216dd74[4] = {
#include "data/main/data_0216dd74.inc.c"
};
void* data_0216dd84[8] = {&data_0216dd30, &data_0216dd64, &data_0216dd58,
                          &data_0216dd40, &data_0216dd2c, &data_0216dda4,
                          &data_0216de70, &data_0216dd28};
char data_0216dda4[85] = {
#include "data/main/data_0216dda4.inc.c"
};
u32 data_0216ddfc[29] = {
#include "data/main/data_0216ddfc.inc.c"
};
char data_0216de70[134] = {
#include "data/main/data_0216de70.inc.c"
};
#pragma explicit_zero_data reset

int wctomb(char* dest, wchar_t wc) {
  return gMslLocale.ctype->wctomb(dest, wc);
}
