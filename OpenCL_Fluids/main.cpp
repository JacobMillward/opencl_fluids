#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include "../nclgl/Renderer.h"
#include "OpenCLUtil.h"


int main() {
	float poolSize = 100;

	cl_int gridWidth = 50;
	cl_float h = poolSize / gridWidth;
	cl_float c2 = 1; // TODO: Figure out what the hell this value is (?Wave propogation speed?)

	/* Lets hardcode the timestep for now until we integrate NCLGL */
	cl_float dt = 0.01f;

	Window w = Window(800, 600);
	Renderer r(w);

	/* Set up OpenCL */
	OpenCLUtil clUtil = OpenCLUtil();

	clUtil.printDeviceInfo(clUtil.getDevice());

	cl::Program* program = clUtil.createProgram("Kernels/Height_Field_Fluid_Sim.cl");

	//Create kernel
	cl::Kernel kernel(*program, "ColumnSimStep");

	/* Create OpenCL buffers */
	cl::Buffer u = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth * gridWidth * sizeof(cl_float));
	cl::Buffer u2 = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth * gridWidth * sizeof(cl_float));
	cl::Buffer v = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth * gridWidth * sizeof(cl_float));

	// TODO: Initialise u with an interesting function

	/* Set kernel arguments */
	kernel.setArg(0, u);
	kernel.setArg(1, u2);
	kernel.setArg(2, v);
	kernel.setArg(3, gridWidth);
	kernel.setArg(4, h);
	kernel.setArg(5, c2);
	kernel.setArg(6, dt);

	/* Set up NDRanges */
	cl::NDRange global(gridWidth * gridWidth);
	cl::NDRange local(1);

	while (w.UpdateWindow()) {
	//	clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	}

	delete program;
	return 0;
}
