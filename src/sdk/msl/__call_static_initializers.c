#include "types.h"

/* Start of the table of static initializers, ended by a null entry. */
extern void (*ARM9_CTOR_START[])(void);

/* Runs every static initializer in the table. Called from Entry. */
void __call_static_initializers(void)
{
    void (**initializer)(void) = ARM9_CTOR_START;

    while (initializer != NULL && *initializer != NULL) {
        (*initializer)();
        initializer++;
    }
}
