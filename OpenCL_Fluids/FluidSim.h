#pragma once
#include "OpenClUtil.h"
#include "../nclgl/RenderObject.h"

class FluidSim
{
public:
	FluidSim(float poolSize, int gridWidth, float c);
	~FluidSim();

	void step(float dt);
	RenderObject getRenderObject() { return renderObject; };

private:
	float poolSize_;
	int gridWidth_;
	float h_;
	float c2_; //TODO: Figure out what the hell this value is (?Wave propogation speed?)
	
	OpenCLUtil clUtil;
	cl::Program* program;
	cl::Kernel kernel;
	cl::NDRange global;
	cl::NDRange local;

	cl::Buffer clBuff_u;
	cl::Buffer clBuff_u2;
	cl::Buffer v;
	cl_float3* host_u;

	Shader* shader;
	Mesh* mesh;
	RenderObject renderObject;

	bool flipBuff = true;
};

