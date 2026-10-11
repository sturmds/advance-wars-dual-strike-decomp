#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

int func_020b983c(void);
u8 *func_021016e4(void);
int func_02124534(const char *a, const char *b);
char *strcpy(char *dest, const char *src);
void Ov006_DrawNameField(void);
void Screens_SelectConfigs(void);
void Battle_RegisterFrameCallbacks(void);
void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_MapFrame(void);
void Camera_ResetToPreset1(void *p);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Map_ReloadPalettes(void);
void Ov006_LoadTerrainPalettes(void);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern u32 gMapGameRunning;
extern u8 data_022a7d5c[];

void func_ov006_023544c4(void)
{
    void (*callback)(int);

    if (func_020b983c() == 1) {
        char *name = (char *)func_021016e4();

        if (func_02124534(name, (char *)data_ov006_0236d640->unk_d8)) {
            strcpy((char *)data_ov006_0236d640->unk_d8, name);
            data_ov006_0236d640->unk_00 |= 0x1000;
        }
        Ov006_DrawNameField();
    }
    Screens_SelectConfigs();
    callback = data_027c027c->unk_50[data_027c0284]->unk_54;
    if (callback != NULL) {
        callback(1);
    }
    Battle_RegisterFrameCallbacks();
    MainLoop_SetFrameCallback(MainLoop_MapFrame);
    gMapGameRunning = 0;
    Camera_ResetToPreset1(data_022a7d5c);
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    Map_ReloadPalettes();
    Ov006_LoadTerrainPalettes();
}
