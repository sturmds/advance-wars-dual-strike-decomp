#ifndef SDK_DESTRUCTOR_CHAIN_H
#define SDK_DESTRUCTOR_CHAIN_H

#include "types.h"

/* One static object that needs its destructor run at exit, or when the
 * overlay holding it is unloaded. The Metrowerks runtime keeps these in a
 * list, newest first. */
typedef struct DestructorChain {
  struct DestructorChain* next;
  void (*destructor)(void* object);
  void* object;
} DestructorChain;

/* Head of the list (__global_destructor_chain in the runtime). */
extern DestructorChain* data_022ad540;

#endif
