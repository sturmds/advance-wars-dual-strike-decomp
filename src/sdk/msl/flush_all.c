#include "types.h"
#include "sdk/msl_file.h"

int __flush_all(void) {
  int result = 0;
  int i = 1;
  FILE* file = gMslFiles;

  do {
    if (file->mode.fileKind != FILE_KIND_CLOSED) {
      if (fflush(file) != 0) {
        result = EOF;
      }
    }
    file = (i < FILE_COUNT) ? &gMslFiles[i++] : NULL;
  } while (file != NULL);

  return result;
}
