#include "types.h"

typedef struct Unk_022a7fc8 Unk_022a7fc8;
typedef struct PlayRecords PlayRecords;

struct Unk_022a7fc8 {
    int unk_0;
    int unk_4;
};

struct PlayRecords {
    int unk_0;
    int unk_4;
};

extern Unk_022a7fc8 data_022a7fc8;
extern PlayRecords gPlayRecords;

void * func_02103c84(void)
{
    data_022a7fc8.unk_0 = gPlayRecords.unk_0;
    data_022a7fc8.unk_4 = gPlayRecords.unk_4;
    return &data_022a7fc8;
}
