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

	glGenBuffers(1, &vbo_u);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_u);
	glBufferData(GL_ARRAY_BUFFER, gridWidth_*gridWidth_ * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &vbo_u2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_u2);
	glBufferData(GL_ARRAY_BUFFER, gridWidth_*gridWidth_ * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	/* Create OpenCL buffers */
	clBuff_u = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth_ * gridWidth_ * sizeof(float));// clUtil.createSharedBuffer(&vbo_u, gridWidth_*gridWidth_, CL_MEM_READ_ONLY);
	clBuff_u2 = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth_ * gridWidth_ * sizeof(float));// clUtil.createSharedBuffer(&vbo_u2, gridWidth_*gridWidth_, CL_MEM_READ_WRITE);
	v = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, gridWidth_ * gridWidth_ * sizeof(float));

	/*gl_buffers = new std::vector<cl::Memory>();
	gl_buffers->push_back(*clBuff_u);
	gl_buffers->push_back(*clBuff_u2);*/

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
	//delete clBuff_u;
	//delete clBuff_u2;

	glDeleteBuffers(1, &vbo_u);
	glDeleteBuffers(1, &vbo_u2);
}

void FluidSim::step(float dt)
{
	/* Acquire OpenGL buffers */
	/*glFinish();
	auto err = clUtil.getCommandQueue().enqueueAcquireGLObjects(gl_buffers);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error aquiring GL buffers (" << err << ")\n";
	}*/
	/* Bind updated values */
	if (flipBuff)
	{
		kernel.setArg(0, clBuff_u);
		kernel.setArg(1, clBuff_u2);

	}
	else
	{
		kernel.setArg(0, clBuff_u2);
		kernel.setArg(1, clBuff_u);

	}
	kernel.setArg(6, dt);

	/* Run kernel */
	auto err = clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error running kernel (" << err << ")\n";
	}

	/* Release OpenGL buffers */
	//clUtil.getCommandQueue().enqueueReleaseGLObjects(gl_buffers);
	/* Wait for command queue to finish */
	clFinish(clUtil.getCommandQueue()());

	// Explicit copy via host because cl/gl interop is being an ass
	// Only copy the changed buffer
	if (flipBuff) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo_u2);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u2, CL_FALSE, 0, gridWidth_*gridWidth_ * sizeof(float), ptr);
		clFinish(clUtil.getCommandQueue()());
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, vbo_u);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u, CL_FALSE, 0, gridWidth_*gridWidth_ * sizeof(float), ptr);
		clFinish(clUtil.getCommandQueue()());
	}
	if (err != CL_SUCCESS)
	{
		std::cout << "Error running kernel (" << err << ")\n";
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_u2);
	float* buf = ((float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
	std::printf("vbo_u2 GL Read Buffer Values: %f, %f, %f , %f\n", buf[0], buf[1], buf[2], buf[3]);
	if (glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) { std::cout << "We done fucked up.\n"; }
	glBindBuffer(GL_ARRAY_BUFFER, vbo_u);
	buf = ((float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
	std::printf("vbo_u GL Read Buffer Values: %f, %f, %f , %f\n", buf[0], buf[1], buf[2], buf[3]);
	if (glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) { std::cout << "We done fucked up.\n"; }

	flipBuff = !flipBuff;
}
