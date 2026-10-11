#ifndef SDK_MSL_FILE_H
#define SDK_MSL_FILE_H

#include "types.h"

/* io_state values of a FILE. */
#define FILE_NEUTRAL 0
#define FILE_WRITING 1
#define FILE_READING 2

#define FILE_KIND_CLOSED 0
#define FILE_IO_READ_ONLY 1

#define EOF (-1)
#define FILE_COUNT 3 /* stdin, stdout, stderr */

typedef struct FileModes {
    u32 openMode : 2;
    u32 ioMode : 3;
    u32 bufferMode : 2;
    u32 fileKind : 3;
    u32 orientation : 2;
    u32 binaryIo : 1;
} FileModes;

typedef struct FileState {
    u32 ioState : 3;
} FileState;

typedef int (*FileIoProc)(u32 handle, u8 *buffer, u32 *count, void *refCon);

/* The MSL C library's stream, 0x4c bytes. */
typedef struct FILE {
    u32 handle;
    FileModes mode;
    FileState state;
    u8 unk_0C;
    u8 error;
    u8 unk_0E[10];
    u32 position;
    u8 *buffer;
    u32 bufferSize;
    u8 *bufferPtr;
    u32 bufferLen;
    u32 bufferAlignment;
    u32 unk_30;
    u32 bufferPos;
    void *positionProc;
    FileIoProc readProc;
    FileIoProc writeProc;
    void *closeProc;
    void *refCon;
} FILE;

extern FILE gMslFiles[FILE_COUNT];

int fflush(FILE *file);
int __flush_all(void);
/* Writes the pending buffer through writeProc; returns the proc's error code, 0 on success. */
int __flush_buffer(FILE *file, u32 *bytesFlushed);
/* Points the buffer at its start, aligned to the file position. */
void __prep_buffer(FILE *file);
void __convert_from_newlines(u8 *buffer, u32 *length);

#endif
