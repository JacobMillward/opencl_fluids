#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL\cl.hpp>
#include <vector>

int main() {

	std::vector<cl::Device> allDevices;
	std::vector<cl::Platform> allPlatforms;
	//Length of vectors
	int n = 10000;

	//Get all platforms
	cl::Platform::get(&allPlatforms);
	if (allPlatforms.size() == 0) {
		std::cout << " No platforms found.\n";
		exit(1);
	}

	for (int i = 0; i < allPlatforms.size(); ++i) {
		//Identify the platform for posterity
		std::cout << "Platform Name: " << allPlatforms[i].getInfo<CL_PLATFORM_NAME>() << "\n";
		std::cout << "Platform Vendor: " << allPlatforms[i].getInfo<CL_PLATFORM_VENDOR>() << "\n\n";
		//Get all GPU devices for this platform
		allPlatforms[i].getDevices(CL_DEVICE_TYPE_GPU, &allDevices);
		if (allDevices.size() == 0) {
			continue;
		}
		else {
			break;
		}
	}

	if (allDevices.size() < 1) {
		std::cout << " No GPU Devices found.\n";
		exit(1);
	}

	//Lets use the first device
	cl::Device defaultDevice = allDevices[0];
	//Identify the device
	std::cout << "Device Name: " << defaultDevice.getInfo<CL_DEVICE_NAME>() << "\n";
	std::cout << "Device Vendor: " << defaultDevice.getInfo<CL_DEVICE_VENDOR>() << "\n";
	std::cout << "Device Max Compute Units: " << defaultDevice.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
	std::cout << "Device Global Memory: " << defaultDevice.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << "\n";
	std::cout << "Device Max Clock Frequency: " << defaultDevice.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << "\n";
	std::cout << "Device Max Allocateable Memory: " << defaultDevice.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << "\n";
	std::cout << "Device Local Memory: " << defaultDevice.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << "\n\n";

	//Create our context
	cl::Context context({ defaultDevice });

	//Read source file
	std::ifstream sourceFile("Kernels/VectorAdd.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

	//Create a program from the source in the context
	cl::Program program = cl::Program(context, source);

	//Build the program for the device
	if (program.build({ defaultDevice }) != CL_SUCCESS) {
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice) << "\n";
		exit(1);
	}

	//Create our command queue
	cl::CommandQueue queue(context, defaultDevice);

	//Create kernel
	cl::Kernel kernel(program, "VectorAdd");

	//Create buffers on device
	cl::Buffer bufferA = cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
	cl::Buffer bufferB = cl::Buffer(context, CL_MEM_READ_ONLY, n * sizeof(int));
	cl::Buffer bufferC = cl::Buffer(context, CL_MEM_WRITE_ONLY, n * sizeof(int));

	//Generate vectors
	int *A = new int[n];
	int *B = new int[n];
	for (int i = 0; i < n; i++) {
		A[i] = std::rand();
		B[i] = n - std::rand();
	}

	//Copy the vectors to memory buffers
	queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, n * sizeof(int), A);
	queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, n * sizeof(int), B);

	//Set kernel arguments
	kernel.setArg(0, bufferA);
	kernel.setArg(1, bufferB);
	kernel.setArg(2, bufferC);

	//Run the kernel on the specified ND range
	cl::NDRange global(n);
	cl::NDRange local(1);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

	//Read buffer into a local list
	int *C = new int[n];
	queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, n * sizeof(int), C);

	std::cout << "First 10 items of buffers:\n";
	for (int i = 0; i < 10; i++) {
		std::cout << A[i] << " + " << B[i] << " = " << C[i] << "\n";
	}

	delete A, B, C;
	return 0;
}
