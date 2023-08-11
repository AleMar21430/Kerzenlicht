#include "device_launch_parameters.h"
#include <cuda_runtime.h>
#include <stdio.h>

__global__ void addArrays(int* a, int* b, int* result, int size) {
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx < size) {
		result[idx] = a[idx] + b[idx];
	}
}