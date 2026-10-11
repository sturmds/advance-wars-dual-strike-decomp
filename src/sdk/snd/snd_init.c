#include "sdk/snd.h"

void func_0210e0c0(void *mutex);
void SND_CommandInit(void);
void func_02110850(void);

void SND_Init(void)
{
    if (data_022a9c50) {
        return;
    }
    data_022a9c50 = 1;
    func_0210e0c0(&data_022a9c54);
    SND_CommandInit();
    func_02110850();
}
