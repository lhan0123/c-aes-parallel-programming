# Introduction

This is the final project of Parallel Programming course (Fall 2021) at National Tsing Hua University (NTHU). The goal is to parallelize the cipher block chaining algorithm to achieve faster cipher speed.

# How to run

We modify the algorithm based on [tiny-AES-c](https://github.com/kokke/tiny-AES-c/blob/master/unlicense.txt](https://github.com/kokke/tiny-AES-c.git) with three techniques pthread on single process, omp on single process, and mp1+omp on multiple processes. 

**Configuration**

- If the number of cipher data blocks exceeds 1024, the host sequentially launches multiple kernels to process the AES decryption. Each kernel is configured with a grid size of 1 and a block size of 1024.  
- If the number of cipher data blocks is 1024 or fewer, the host launches a single kernel to handle the decryption. The kernel is configured with a grid size of 1 and a block size equal to the number of cipher data blocks.  
- Each thread is responsible for handling the AES decryption of one cipher data block.

**Optimizations Applied**

- Coalesced Memory Access: This optimization ensures that memory accesses are efficient and minimize the number of transactions.
- Shared Memory: Although used, it does not significantly contribute to performance due to limited data reuse opportunities.
- Loop Unrolling: This technique is applied to reduce the overhead of loop control and increase execution efficiency.

```
# pthread
g++ test.c -o test -lm aes.h aes.c -pthread

# omp-single
mpicxx test.c -o test -lm aes.h aes.c -fopenmp

# omp-multiple
mpicxx test.c -o test -lm aes.h aes.c -fopenmp
```

# Analysis

- Without the use of shared memory, the AES decryption execution time is 0.175348 seconds.
- With the use of shared memory, the execution time is reduced to 0.118362 seconds.
- This analysis shows that while shared memory usage provides some performance benefits, other optimizations like coalesced memory access and loop unrolling also play crucial roles in enhancing the efficiency of the AES decryption process on a GPU.
