#include "types.h"
#include "sdk/msl_file.h"

int fflush(FILE *file)
{
    if (file == NULL) {
        return __flush_all();
    }

    if (file->error || file->mode.fileKind == FILE_KIND_CLOSED) {
        return EOF;
    }

    if (file->mode.ioMode == FILE_IO_READ_ONLY) {
        return 0;
    }

    if (file->state.ioState >= FILE_REREADING) {
        file->state.ioState = FILE_READING;
    }

    if (file->state.ioState == FILE_READING) {
        file->bufferLen = 0;
    }

    if (file->state.ioState != FILE_WRITING) {
        file->state.ioState = FILE_NEUTRAL;
        return 0;
    }

    if (__flush_buffer(file, NULL) != 0) {
        file->error = 1;
        file->bufferLen = 0;
        return EOF;
    }

    file->state.ioState = FILE_NEUTRAL;
    file->position = 0;
    file->bufferLen = 0;
    return 0;
}
