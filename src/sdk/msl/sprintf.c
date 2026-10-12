#include "stdarg.h"
#include "types.h"
#include "sdk/msl.h"

int sprintf(char* buf, const char* format, ...) {
  va_list args;

  va_start(args, format);
  return vsnprintf(buf, -1, format, args);
}
