#pragma once
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL\cl.hpp>
#include <string>
#include <Windows.h>

class OpenCLUtil
{
private:
	cl::Platform platform_;
	cl::Device device_;
	cl::Context context_;
	cl::CommandQueue queue_;

public:
	OpenCLUtil();
	~OpenCLUtil();

	cl::Device& getDevice() {
		return device_;
	};
	cl::Context& getContext() {
		return context_;
	};
	cl::CommandQueue& getCommandQueue() {
		return queue_;
	};
	cl::Program* createProgram(std::string filePath);
	cl::BufferGL* createSharedBuffer(GLuint* vbo, size_t size, cl_mem_flags flags);
	void printDeviceInfo(cl::Device device);
};

