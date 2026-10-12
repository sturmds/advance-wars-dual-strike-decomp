#include "types.h"
#include "sdk/fs.h"

extern int data_022a7e68;
extern const char data_0216a5b8[];
extern u8 data_0218ff58[];
extern u8 data_022a7e88[];
int Wireless_PickChannel(void);
void func_0211c0d8(void);
u32 DownloadPlay_ProgramBufferSize(FSFile* file);
BOOL DownloadPlay_LoadChildProgram(FSFile* file, void* dest, u32 size);
BOOL DownloadPlay_OfferChildProgram(void* dest, void* src);

/* Needed to match: the original builds this test as a 0/1 value first, as an
 * inline function does. */
static inline BOOL IsFile(FSFile* file) {
  if (file->flags & FS_FILE_STATUS_IS_FILE) {
    return TRUE;
  }
  return FALSE;
}

BOOL func_020fc89c(void) {
  FSFile file;
  u32 size;

  if (Wireless_PickChannel()) {
    func_0211c0d8();
    FS_InitFile(&file);
    if (FS_OpenFile(&file, data_0216a5b8)) {
      size = DownloadPlay_ProgramBufferSize(&file);
      if (size != 0 && size <= 0x100000) {
        if (DownloadPlay_LoadChildProgram(&file, data_0218ff58, size)) {
          if (DownloadPlay_OfferChildProgram(data_022a7e88, data_0218ff58)) {
            if (IsFile(&file) != FALSE) {
              FS_CloseFile(&file);
            }
            data_022a7e68 = 2;
            return TRUE;
          }
        }
      }
    }
  }
  if (IsFile(&file) != FALSE) {
    FS_CloseFile(&file);
  }
  return FALSE;
}
