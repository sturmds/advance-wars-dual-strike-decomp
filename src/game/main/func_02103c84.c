#include "types.h"

typedef struct Unk_022a7fc8 Unk_022a7fc8;
typedef struct Unk_02290718 Unk_02290718;

struct Unk_022a7fc8 {
    int unk_0;
    int unk_4;
};

struct Unk_02290718 {
    int unk_0;
    int unk_4;
};

extern Unk_022a7fc8 data_022a7fc8;
extern Unk_02290718 data_02290718;

void * func_02103c84(void)
{
    data_022a7fc8.unk_0 = data_02290718.unk_0;
    data_022a7fc8.unk_4 = data_02290718.unk_4;
    return &data_022a7fc8;
}
