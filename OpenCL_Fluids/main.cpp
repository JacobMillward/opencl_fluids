#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include "OpenCLUtil.h"

int main() {

	std::vector<cl::Device> allDevices;
	std::vector<cl::Platform> allPlatforms;
	//Length of vectors
	int n = 10000;

	OpenCLUtil clUtil = OpenCLUtil();

	clUtil.printDeviceInfo();

	cl::Program* program = clUtil.createProgram("Kernels/VectorAdd.cl");

	//Create kernel
	cl::Kernel kernel(*program, "VectorAdd");

	//Create buffers on device
	cl::Buffer bufferA = cl::Buffer(clUtil.getContext(), CL_MEM_READ_ONLY, n * sizeof(int));
	cl::Buffer bufferB = cl::Buffer(clUtil.getContext(), CL_MEM_READ_ONLY, n * sizeof(int));
	cl::Buffer bufferC = cl::Buffer(clUtil.getContext(), CL_MEM_WRITE_ONLY, n * sizeof(int));

	//Generate vectors
	int *A = new int[n];
	int *B = new int[n];
	for (int i = 0; i < n; i++) {
		A[i] = std::rand();
		B[i] = n - std::rand();
	}

	//Copy the vectors to memory buffers
	clUtil.getCommandQueue().enqueueWriteBuffer(bufferA, CL_TRUE, 0, n * sizeof(int), A);
	clUtil.getCommandQueue().enqueueWriteBuffer(bufferB, CL_TRUE, 0, n * sizeof(int), B);

	//Set kernel arguments
	kernel.setArg(0, bufferA);
	kernel.setArg(1, bufferB);
	kernel.setArg(2, bufferC);

	//Run the kernel on the specified ND range
	cl::NDRange global(n);
	cl::NDRange local(1);
	clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

	//Read buffer into a local list
	int *C = new int[n];
	clUtil.getCommandQueue().enqueueReadBuffer(bufferC, CL_TRUE, 0, n * sizeof(int), C);

	std::cout << "First 10 items of buffers:\n";
	for (int i = 0; i < 10; i++) {
		std::cout << A[i] << " + " << B[i] << " = " << C[i] << "\n";
	}

	delete A, B, C;
	return 0;
}
