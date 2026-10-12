#include "types.h"
#include "sdk/msl_wchar.h"

int wctomb(char* dest, wchar_t wc) {
  return gMslLocale.ctype->wctomb(dest, wc);
}
