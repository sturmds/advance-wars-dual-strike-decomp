#include "types.h"
#include "sdk/msl_file.h"

int __flush_buffer(FILE* file, u32* bytesFlushed) {
  u32 pending = file->bufferPtr - file->buffer;
  int error;

  if (pending != 0) {
    file->bufferLen = pending;
    if (!file->mode.binaryIo) {
      __convert_from_newlines(file->buffer, &file->bufferLen);
    }
    error = file->writeProc(file->handle, file->buffer, &file->bufferLen,
                            file->refCon);
    if (bytesFlushed != NULL) {
      *bytesFlushed = file->bufferLen;
    }
    if (error != 0) {
      return error;
    }
    file->position += file->bufferLen;
  }
  __prep_buffer(file);
  return 0;
}
