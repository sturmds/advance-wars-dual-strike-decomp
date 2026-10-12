#include "types.h"
#include "sdk/destructor_chain.h"

/* The runtime calls each destructor with a second argument of -1. */
typedef void (*DestructorWithFlag)(void* object, int flag);

/* Runs the destructors of all static objects, newest first. Called from exit. */
void __destroy_global_chain(void) {
  DestructorChain* chain;

  while ((chain = data_022ad540) != NULL) {
    data_022ad540 = chain->next;
    ((DestructorWithFlag)chain->destructor)(chain->object, -1);
  }
}
