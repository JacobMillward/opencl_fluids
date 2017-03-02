#pragma once
#include "OpenClUtil.h"

class FluidSim
{
public:
	FluidSim(float poolSize, int gridWidth, float c);
	~FluidSim();

	void step(float dt);
private:
	float poolSize_;
	int gridWidth_;
	float h_;
	float c2_; //TODO: Figure out what the hell this value is (?Wave propogation speed?)

	GLuint vbo_u;
	GLuint vbo_u2;

	cl::Buffer clBuff_u;
	cl::Buffer clBuff_u2;
	cl::Buffer v;

	OpenCLUtil clUtil;
	cl::Program* program;
	cl::Kernel kernel;
	cl::NDRange global;
	cl::NDRange local;

	bool flipBuff;
};

