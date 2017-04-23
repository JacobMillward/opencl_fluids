#pragma once
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>

class OpenCLUtil
{
private:
	cl::Platform platform_;
	cl::Device device_;
	cl::Context context_;
	cl::CommandQueue queue_;

public:
	OpenCLUtil(cl_device_type type);
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
	cl::Program* createProgram(std::string filePath) const;
	cl::BufferGL* createSharedBuffer(GLuint* vbo, size_t size, cl_mem_flags flags) const;
	static void printDeviceInfo(cl::Device device);
};

