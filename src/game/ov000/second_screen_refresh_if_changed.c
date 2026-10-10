#include "types.h"

BOOL func_ov000_022b03d8(void);
void func_ov000_022b2d48(void);
void SecondScreen_DrawTerrainPanel(int a, int b);

extern int data_ov000_02350530;
extern int data_ov000_02350534;
extern int data_ov000_02350538;
extern int data_ov000_0235053c;
extern int data_ov000_02350540;
extern int data_ov000_02350544;
extern int data_ov000_0235054c;

/* Second screen: when func_ov000_022b03d8 reports that the requested values differ from the
 * drawn ones, copies them over and redraws, then calls SecondScreen_DrawTerrainPanel either way. */
void SecondScreen_RefreshIfChanged(void)
{
    if (func_ov000_022b03d8()) {
        data_ov000_02350530 = data_ov000_02350538;
        data_ov000_0235054c = data_ov000_02350534;
        data_ov000_0235053c = data_ov000_02350544;
        func_ov000_022b2d48();
    }
    SecondScreen_DrawTerrainPanel(data_ov000_02350540, data_ov000_02350530);
}
