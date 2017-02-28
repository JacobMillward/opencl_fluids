#include "FluidSim.h"


FluidSim::FluidSim(float poolSize, int gridWidth, float c) : poolSize_(poolSize), gridWidth_(gridWidth), h_(poolSize/gridWidth), c2_(c*c)
{
	/* Set up OpenCL */
	clUtil = OpenCLUtil();

	clUtil.printDeviceInfo(clUtil.getDevice());

	program = clUtil.createProgram("Kernels/Height_Field_Fluid_Sim.cl");

	//Create kernel
	kernel = cl::Kernel(*program, "ColumnSimStep");

	/* Create OpenCL buffers */
	cl::Buffer u = cl::Buffer(clUtil.getContext(), CL_MEM_HOST_NO_ACCESS, gridWidth * gridWidth * sizeof(cl_float));
	cl::Buffer u2 = cl::Buffer(clUtil.getContext(), CL_MEM_HOST_NO_ACCESS, gridWidth * gridWidth * sizeof(cl_float));
	cl::Buffer v = cl::Buffer(clUtil.getContext(), CL_MEM_HOST_NO_ACCESS, gridWidth * gridWidth * sizeof(cl_float));

	// TODO: Initialise u with an interesting function

	/* Set kernel arguments */
	kernel.setArg(0, u);
	kernel.setArg(1, u2);
	kernel.setArg(2, v);
	kernel.setArg(3, gridWidth_);
	kernel.setArg(4, h_);
	kernel.setArg(5, c2_);

	/* Set up NDRanges */
	global = cl::NDRange(gridWidth * gridWidth);
	local = cl::NDRange(1);
}


FluidSim::~FluidSim()
{
	delete program;
}

void FluidSim::step(float dt)
{
	/* Bind dt for updated value */
	kernel.setArg(6, dt);
	clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	clFinish(clUtil.getCommandQueue()());
}
