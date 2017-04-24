#pragma once
#include "OpenClUtil.h"
#include "../nclgl/RenderObject.h"
#include "FluidMesh.h"
#include <chrono>

typedef common_type<chrono::duration<long long, ratio<1, 1000000000>>, chrono::duration<long long, ratio<1, 1000000000>>>::type TimeDifference;

class FluidSim
{
public:
	FluidSim(float poolSize, int gridWidth, float c, float maxSlope, cl_device_type deviceType, string texture = "water.png");
	~FluidSim();

	void step(float dt);
	RenderObject getFluidRenderObject() const { return fluidRenderObject; };
	RenderObject getCubeRenderObject() const { return cubeRenderObject; };

	TimeDifference getKernelExecutionTime() const;
	TimeDifference getBufferCopyExecutionTime() const;

private:
	float poolSize_;
	int gridWidth_;
	float h_;
	float c2_;
	float maxSlope_;

	OpenCLUtil clUtil;
	cl::Program* program;
	cl::Kernel kernel;
	cl::NDRange global;
	cl::NDRange local;

	cl::Buffer clBuff_u;
	cl::Buffer clBuff_u2;
	cl::Buffer v;
	cl_float* host_u;

	Shader* fluidShader;
	FluidMesh* fluidMesh;
	Mesh* cubeMesh;
	GLuint fluidTexture;
	RenderObject fluidRenderObject;
	RenderObject cubeRenderObject;

	bool flipBuff = true;
	TimeDifference kernelExecutionTime;
	TimeDifference bufferCopyExecutionTime;
};
