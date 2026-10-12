#include "types.h"
#include "sdk/msl.h"

int __StringWrite(void* context, const char* src, u32 count) {
  StringSink* sink = context;

  if (sink->length + count > sink->capacity) {
    count = sink->capacity - sink->length;
  }
  __copy_mem(sink->buffer + sink->length, src, count);
  sink->length += count;
  return 1;
}
