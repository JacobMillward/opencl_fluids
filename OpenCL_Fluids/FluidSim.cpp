#include "GL/glew.h"
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

	/* Create OpenCL buffers */
	clBuff_u = clUtil.createSharedBuffer(&vbo_u, gridWidth_*gridWidth_, CL_MEM_WRITE_ONLY);
	clBuff_u2 = clUtil.createSharedBuffer(&vbo_u2, gridWidth_*gridWidth_, CL_MEM_WRITE_ONLY);

	v = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth_ * gridWidth_ * sizeof(float));

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

	glDeleteBuffers(1, &vbo_u);
	glDeleteBuffers(1, &vbo_u2);
}

void FluidSim::step(float dt)
{
	/* Acquire OpenGL buffers */
	glFlush();
	auto err = clUtil.getCommandQueue().enqueueAcquireGLObjects(gl_buffers);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error aquiring GL buffers (" << err << ")\n";
	}
	/* Bind updated values */
	/*if (flipBuff)
	{*/
		kernel.setArg(0, *clBuff_u);
		kernel.setArg(1, *clBuff_u2);

	/*}
	else
	{
		kernel.setArg(0, *clBuff_u2);
		kernel.setArg(1, *clBuff_u);

	}
	flipBuff = !flipBuff;*/
	kernel.setArg(6, dt);
	/* Run kernel */
	err = clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error running kernel (" << err << ")\n";
	}
	/* Release OpenGL buffers */
	clUtil.getCommandQueue().enqueueReleaseGLObjects(gl_buffers);
	/* Wait for command queue to finish */
	clFinish(clUtil.getCommandQueue()());
	glBindBuffer(GL_ARRAY_BUFFER, vbo_u2);
	float* buf = ((float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
	std::printf("GL Read Buffer Values: %f, %f, %f , %f\n", buf[0], buf[1], buf[2], buf[3]);
	if (glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) { std::cout << "We done fucked up.\n"; }
}
