#ifndef CUDA_UTILS_H
#define CUDA_UTILS_H

#include <stdio.h>
#include <cmath>
#include <cuda.h>
#include <cuda_runtime.h>
#include <vector>
#define TOTAL_THREADS 512
inline int opt_n_threads(int work_size)
{
    const int pow_2 = std::log(static_cast<double>(work_size)) / std::log(2.0);
    return std::max(std::min(1 << pow_2, TOTAL_THREADS), 1);
}

inline dim3 opt_block_config(int x, int y)
{
    const int x_threads = opt_n_threads(x);
    const int y_threads = std::max(std::min(opt_n_threads(y), TOTAL_THREADS / x_threads), 1);
    dim3 block_config(x_threads, y_threads, 1);
    return block_config;
}

#define CUDA_CHECK_ERRORS()                                           \
  do {                                                                \
    cudaError_t err = cudaGetLastError();                             \
    if (cudaSuccess != err) {                                         \
      fprintf(stderr, "CUDA kernel failed : %s\n%s at L:%d in %s\n",  \
              cudaGetErrorString(err), __PRETTY_FUNCTION__, __LINE__, \
              __FILE__);                                              \
      exit(-1);                                                       \
    }                                                                 \
  } while (0)

struct inno_point
{
  float x;
  float y;
  float z;
};


// Helper function for serializing plugin
template<typename T>
void writeToBuffer(char*& buffer, const T& val)
{
    *reinterpret_cast<T*>(buffer) = val;
    buffer += sizeof(T);
}

// Helper function for deserializing plugin
template<typename T>
T readFromBuffer(const char*& buffer)
{
    T val = *reinterpret_cast<const T*>(buffer);
    buffer += sizeof(T);
    return val;
}
#endif // CUDA_UTILS_H
