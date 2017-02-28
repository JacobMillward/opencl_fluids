#pragma once
#include <CL\cl_platform.h>

class FluidSim
{
public:
	FluidSim(float poolSize, int gridWidth, float c);
	~FluidSim();

	void step(float dt);
private:
	float poolSize_;
	cl_int gridWidth_;
	cl_float h_;
	cl_float c2_; //TODO: Figure out what the hell this value is (?Wave propogation speed?)

	OpenCLUtil clUtil;
	cl::Program* program;
	cl::Kernel kernel;
	cl::NDRange global;
	cl::NDRange local;
};

