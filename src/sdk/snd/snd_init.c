#include "sdk/snd.h"

void func_0210e0c0(void *mutex);
void func_02110688(void);
void func_02110850(void);

void SND_Init(void)
{
    if (data_022a9c50) {
        return;
    }
    data_022a9c50 = 1;
    func_0210e0c0(&data_022a9c54);
    func_02110688();
    func_02110850();
}
