#include "types.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void func_01ff8c84(int a);
void Gfx_RequestLayerCopy(int bg);
void func_020c4018(void);
void func_ov006_023505c4(void);

void Ov006_PrepareScriptScreen(void)
{
    PopupMenu_PushState();
    func_0201e954();
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    func_ov006_023505c4();
}
