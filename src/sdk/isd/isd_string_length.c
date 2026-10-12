#include "types.h"
#include "sdk/isd.h"

int Isd_StringLength(const char* s) {
  int n = 0;

  while (Isd_ReadByte(s) != 0) {
    n++;
    s++;
  }
  return n;
}
