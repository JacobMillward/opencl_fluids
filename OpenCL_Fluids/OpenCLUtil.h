#pragma once
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL\cl.hpp>
#include <string>

class OpenCLUtil
{
private:
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
	void printDeviceInfo();
};

