// omp_atomic.cpp
// compile with: /openmp
#include <stdio.h>
#include <omp.h>

#define MAX 10

int main() {
    int count = 0;
#pragma omp parallel num_threads(MAX)
    {
#pragma omp atomic
        count++;
    }
    printf_s("Number of threads: %d\n", count);
}