#include "FluidSim.h"
#include <iostream>

FluidSim::FluidSim(float poolSize, int gridWidth, float c) : poolSize_(poolSize), gridWidth_(gridWidth), h_(poolSize/gridWidth), c2_(c*c)
{
	/* Set up OpenCL */
	clUtil = OpenCLUtil();

	clUtil.printDeviceInfo(clUtil.getDevice());

	program = clUtil.createProgram("Kernels/Height_Field_Fluid_Sim.cl");

	/* Create kernel */
	kernel = cl::Kernel(*program, "ColumnSimStep");

	/* Create VBOs */
	/* Create OpenCL buffers */
	clBuff_u = clUtil.createSharedBuffer(&vbo_u, gridWidth_*gridWidth_*sizeof(float), CL_MEM_READ_WRITE);
	clBuff_u2 = clUtil.createSharedBuffer(&vbo_u2, gridWidth_*gridWidth_*sizeof(float), CL_MEM_READ_WRITE);
	v = cl::Buffer(clUtil.getContext(), CL_MEM_HOST_NO_ACCESS, gridWidth * gridWidth * sizeof(cl_float));

	gl_buffers = new std::vector<cl::Memory>();
	gl_buffers->push_back(*clBuff_u);
	gl_buffers->push_back(*clBuff_u2);

	// TODO: Initialise u with an interesting function

	/* Set kernel arguments */
	kernel.setArg(2, v);
	kernel.setArg(3, gridWidth_);
	kernel.setArg(4, h_);
	kernel.setArg(5, c2_);

	/* Set up NDRanges */
	global = cl::NDRange(gridWidth_ * gridWidth_);
	local = cl::NDRange(1);
}


FluidSim::~FluidSim()
{
	delete program;
	delete gl_buffers;
	delete clBuff_u;
	delete clBuff_u2;
}

void FluidSim::step(float dt)
{
	/* Acquire OpenGL buffers */
	clUtil.getCommandQueue().enqueueAcquireGLObjects(gl_buffers);
	/* Bind updated values */
	if (flipBuff)
	{
		kernel.setArg(0, *clBuff_u);
		kernel.setArg(1, *clBuff_u2);

	}
	else
	{
		kernel.setArg(0, *clBuff_u2);
		kernel.setArg(1, *clBuff_u);

	}
	flipBuff = !flipBuff;
	kernel.setArg(6, dt);
	/* Run kernel */
	if (auto err = clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local) != CL_SUCCESS)
	{
		std::cout << "Error running kernel (" << err << ")\n";
	}
	/* Release OpenGL buffers */
	clUtil.getCommandQueue().enqueueReleaseGLObjects(gl_buffers);
	/* Wait for command queue to finish */
	clFinish(clUtil.getCommandQueue()());
}
