#include "types.h"
#include "sdk/fs.h"

void func_02112bb4(FSFile *file);                            /* FS_InitFile */
BOOL func_02112864(FSFile *file, const char *path);          /* FS_OpenFile */
s32 func_02112718(FSFile *file, void *dest, s32 size);       /* FS_ReadFile */
BOOL func_0211281c(FSFile *file);                            /* FS_CloseFile */
void *func_0200bf40(s32 size);                               /* allocates from the game heap */
void Mem_Free(void *block);
void func_0210a424(volatile u16 *reg, int brightness);       /* sets a master brightness register */
void func_0210eb88(void);                                    /* halts for good */

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)

/* Reads a whole file into a new heap block and returns it. If the file
 * cannot be opened or read, turns the main screen bright and halts. */
void *File_Load(const char *path)
{
    FSFile file;
    s32 size;
    void *data;

    func_02112bb4(&file);
    if (func_02112864(&file, path)) {
        size = file.bottom - file.top;
        data = func_0200bf40(size);
        if (size != func_02112718(&file, data, size)) {
            func_0211281c(&file);
            Mem_Free(data);
            func_0210a424(REG_MASTER_BRIGHT, 12);
            func_0210eb88();
            return NULL;
        }
        func_0211281c(&file);
    } else {
        func_0210a424(REG_MASTER_BRIGHT, 12);
        func_0210eb88();
        return NULL;
    }
    return data;
}
