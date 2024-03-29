#include <GL/glew.h>
#include "OpenCLUtil.h"
#include <iostream>
#include <fstream>
#include <vector>

OpenCLUtil::OpenCLUtil(cl_device_type type)
{
	/* Search all platforms for a GPU device */

	std::vector<cl::Device> allDevices;
	std::vector<cl::Platform> allPlatforms;
	cl::Platform::get(&allPlatforms);
	if (allPlatforms.size() == 0)
	{
		throw "No platforms found!";
	}

	for (size_t i = 0; i < allPlatforms.size(); ++i)
	{

		allPlatforms[i].getDevices(type, &allDevices);

		if (allDevices.size() > 0)
		{
			//Identify the platform for posterity
			std::cout << "Platform Name: " << allPlatforms[i].getInfo<CL_PLATFORM_NAME>() << "\n";
			std::cout << "Platform Vendor: " << allPlatforms[i].getInfo<CL_PLATFORM_VENDOR>() << "\n";

			this->platform_ = allPlatforms[i];
			break;
		}
	}

	if (allDevices.size() < 1)
	{
		throw "No Devices found.";
	}

	// Magic fuckery going on here.
	//cl_context_properties props[] =
	//{
	//	//OpenCL platform
	//	CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>((this->platform_)()),
	//	//OpenGL context
	//	CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentContext()),
	//	//HDC used to create the OpenGL context
	//	CL_WGL_HDC_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentDC()),
	//	0
	//};

	/* Set device */
	this->device_ = allDevices[0];
	//printDeviceInfo({ this->device_ });

	/* Create OpenCL Context */
	this->context_ = cl::Context(this->device_);

	

	/* Create command queue */
	this->queue_ = cl::CommandQueue(this->context_, this->device_);
}

OpenCLUtil::~OpenCLUtil()
{
}

cl::Program* OpenCLUtil::createProgram(std::string filePath) const
{
	//Read source file
	std::ifstream sourceFile(filePath);
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));
	//for (auto s : source) { printf("%s\n", s.first); }
	//Create a program from the source in the context
	cl::Program* program = new cl::Program(this->context_, source);

	//Build the program for the device
	if (program->build({this->device_}) != CL_SUCCESS)
	{
		std::cout << "Error building program.\n---Build Log---\n" << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->device_) << "\n";
	}

	return program;
}

cl::BufferGL* OpenCLUtil::createSharedBuffer(GLuint* vbo, size_t size, cl_mem_flags flags) const
{
	//Create buffer object
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);

	//Initialize buffer object
	//float test[4] = {0, 1, 2, 3};
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	//Create OpenCL buffer from GL VBO
	cl_int err;
	auto b = new cl::BufferGL(this->context_, flags, *vbo, &err);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error creating shared buffer(" << err << ")\n";
	}
	return b;
}

void OpenCLUtil::printDeviceInfo(cl::Device device)
{
	CL_DEVICE_TYPE_CPU;
	CL_DEVICE_TYPE_GPU;
	std::string deviceType = {};

	switch (device.getInfo<CL_DEVICE_TYPE>())
	{
	case CL_DEVICE_TYPE_ACCELERATOR:
		deviceType = "Accelerator";
		break;
	case CL_DEVICE_TYPE_CPU:
		deviceType = "CPU";
		break;
	case CL_DEVICE_TYPE_GPU:
		deviceType = "GPU";
		break;
	default:
		deviceType = "Unknown";
	}
	/* Identify the device */
	std::cout << "Device Type: " << deviceType << "\n";
	std::cout << "Device Name: " << device.getInfo<CL_DEVICE_NAME>() << "\n";
	std::cout << "Device Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\n";
	std::cout << "Device Version: " << device.getInfo<CL_DEVICE_VERSION>() << "\n";
	std::cout << "Device Max Compute Units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
	std::cout << "Device Global Memory: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << "\n";
	std::cout << "Device Max Clock Frequency: " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << "\n";
	std::cout << "Device Max Allocateable Memory: " << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << "\n";
	std::cout << "Device Local Memory: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << "\n\n";
}
