#include "base.hpp"

int MallocAlignedMemory(void *& memory, size_t memory_size) {
#if defined(_WIN32)
  memory = _aligned_malloc(memory_size, kMemAlignStep);
  if (memory) {
    // malloc success
    return 0;
  }
#elif defined(__ANDROID__)
  memory = memalign(kMemAlignStep, memory_size);
  if (memory) {
    // malloc success
    return 0;
  }
#elif defined (__linux__)
  int ret = posix_memalign(&memory, kMemAlignStep, memory_size);
  if (ret == 0) {
    return ret;
  }
#else
  ERROR_INFO("MallocAlignedMemory not implemented yet!");
#endif //defined(_WIN32) || defined(_WIN64)
  return -1;
}

void FreeAlignedMemory(void *& memory) {
  if (memory) {
#if defined(_WIN32)
    _aligned_free(memory);
#elif defined (__linux__)
    free(memory);
#else
    ERROR_INFO("FreeAlignedMemory not implemented yet!");
#endif //defined(_WIN32) || defined(_WIN64)
    memory = NULL;
  }
}
