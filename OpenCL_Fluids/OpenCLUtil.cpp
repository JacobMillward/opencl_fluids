#include "OpenCLUtil.h"
#include <iostream>
#include <fstream>

OpenCLUtil::OpenCLUtil(HGLRC renderContext, HDC deviceContext)
{
	/* Search all platforms for a GPU device */

	std::vector<cl::Device> allDevices;
	std::vector<cl::Platform> allPlatforms;
	cl::Platform::get(&allPlatforms);
	if (allPlatforms.size() == 0) {
		throw "No platforms found!";
	}

	for (size_t i = 0; i < allPlatforms.size(); ++i) {
		//Get all GPU devices for this platform
		allPlatforms[i].getDevices(CL_DEVICE_TYPE_GPU, &allDevices);
		if (allDevices.size() == 0) {
			continue;
		}
		else {
			//Identify the platform for posterity
			std::cout << "Platform Name: " << allPlatforms[i].getInfo<CL_PLATFORM_NAME>() << "\n";
			std::cout << "Platform Vendor: " << allPlatforms[i].getInfo<CL_PLATFORM_VENDOR>() << "\n";
			this->platform_ = allPlatforms[i];
			break;
		}
	}

	if (allDevices.size() < 1) {
		throw "No GPU Devices found.";
	}

	/* Set device */
	this->device_ = allDevices[0];

	// Magic fuckery going on here.
	cl_context_properties props[] =
	{
		//OpenCL platform
		CL_CONTEXT_PLATFORM, (cl_context_properties)(this->platform_)(),
		//OpenGL context
		CL_GL_CONTEXT_KHR,   (cl_context_properties)renderContext,
		//HDC used to create the OpenGL context
		// I have a feeling this is just coincidence atm, lets fix this later
		CL_WGL_HDC_KHR,     (cl_context_properties)(this->device_)(),
		0
	};
	/* Create OpenCL Context */
	this->context_ = cl::Context(CL_DEVICE_TYPE_GPU, props);

	/* Create command queue */
	this->queue_ = cl::CommandQueue(this->context_, this->device_);
}

OpenCLUtil::~OpenCLUtil()
{
}

cl::Program * OpenCLUtil::createProgram(std::string filePath)
{
	//Read source file
	std::ifstream sourceFile("Kernels/VectorAdd.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

	//Create a program from the source in the context
	cl::Program* program = new cl::Program(this->context_, source);

	//Build the program for the device
	if (program->build({ this->device_ }) != CL_SUCCESS) {
		std::cout << "Error building: " << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->device_) << "\n";
		throw "Error building program.";
	}

	return program;
}

void OpenCLUtil::printDeviceInfo()
{
	/* Identify the device */
	std::cout << "Device Name: " << this->device_.getInfo<CL_DEVICE_NAME>() << "\n";
	std::cout << "Device Vendor: " << this->device_.getInfo<CL_DEVICE_VENDOR>() << "\n";
	std::cout << "Device Max Compute Units: " << this->device_.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
	std::cout << "Device Global Memory: " << this->device_.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << "\n";
	std::cout << "Device Max Clock Frequency: " << this->device_.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << "\n";
	std::cout << "Device Max Allocateable Memory: " << this->device_.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << "\n";
	std::cout << "Device Local Memory: " << this->device_.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << "\n";
}

