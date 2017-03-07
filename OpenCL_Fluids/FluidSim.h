#pragma once
#include "OpenClUtil.h"
#include "../nclgl/RenderObject.h"
#include "FluidMesh.h"

class FluidSim
{
public:
	FluidSim(float poolSize, int gridWidth, float c, float maxSlope, std::string texture = "water.png");
	~FluidSim();

	void step(float dt);
	RenderObject getFluidRenderObject() { return fluidRenderObject; };
	RenderObject getCubeRenderObject() { return cubeRenderObject; };
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
};

