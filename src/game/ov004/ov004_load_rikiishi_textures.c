#include "types.h"

void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void File_ReadTo(void *dest, const char *fileName);
void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void GX_BeginLoadTex(void);
void func_0210c9a0(const void *src, u32 offset, u32 size);  /* guess: copies to texture VRAM, between begin and end calls */
void GX_EndLoadTex(void);
void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(const void *src, u32 offset, u32 size);  /* guess: copies to texture palette VRAM */
void GX_EndLoadTexPltt(void);

extern u8 *data_ov004_022bf150;
extern char gRikiishiDir[];
extern char gRikiishiFile0a4[];
extern char gRikiishiFile0a6[];
extern char gRikiishiFile0a7[];
extern char gRikiishiFile0aa[];
extern char gRikiishiFile0ab[];
extern char gRikiishiFile0ac[];
extern char gRikiishiFile0ad[];
extern char gRikiishiFile0ae[];
extern char gRikiishiFile0af[];
extern char gRikiishiFile0b0[];
extern char gRikiishiFile0a5[];
extern char gRikiishiFile0a8[];
extern char gRikiishiFile0b1[];

void Ov004_LoadRikiishiTextures(void *state)  /* state is not used */
{
    u8 *buffer = data_ov004_022bf150;

    FS_ChangeDir(gRikiishiDir);

    File_UnpackTo(buffer, gRikiishiFile0a4);
    DC_FlushRange(buffer, 0x8000);
    GX_BeginLoadTex();
    func_0210c9a0(buffer, 0, 0x8000);
    GX_EndLoadTex();

    File_UnpackTo(buffer, gRikiishiFile0a6);
    File_UnpackTo(buffer + 0x4000, gRikiishiFile0a7);
    DC_FlushRange(buffer, 0x8000);
    GX_BeginLoadTex();
    func_0210c9a0(buffer, 0x8000, 0x8000);
    GX_EndLoadTex();

    File_UnpackTo(buffer, gRikiishiFile0aa);
    File_UnpackTo(buffer + 0x1000, gRikiishiFile0ab);
    File_UnpackTo(buffer + 0x2000, gRikiishiFile0ac);
    File_UnpackTo(buffer + 0x3000, gRikiishiFile0ad);
    File_UnpackTo(buffer + 0x4000, gRikiishiFile0ae);
    File_UnpackTo(buffer + 0x5000, gRikiishiFile0af);
    File_UnpackTo(buffer + 0x6000, gRikiishiFile0b0);
    DC_FlushRange(buffer, 0x8000);
    GX_BeginLoadTex();
    func_0210c9a0(buffer, 0x10000, 0x8000);
    GX_EndLoadTex();

    File_ReadTo(buffer, gRikiishiFile0a5);
    File_ReadTo(buffer + 0xa0, gRikiishiFile0a8);
    File_ReadTo(buffer + 0xc0, gRikiishiFile0b1);
    DC_FlushRange(buffer, 0xe0);
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(buffer, 0, 0xe0);
    GX_EndLoadTexPltt();
}
