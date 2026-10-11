#include "types.h"
#include "sdk/msl_file.h"

void __prep_buffer(FILE *file)
{
    file->bufferPtr = file->buffer;
    file->bufferLen = file->bufferSize;
    file->bufferLen -= file->position & file->bufferAlignment;
    file->bufferPos = file->position;
}
