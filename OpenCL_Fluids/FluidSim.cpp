#include "GL/glew.h"
#include "FluidSim.h"
#include <iostream>

FluidSim::FluidSim(float poolSize, int gridWidth, float c, float maxSlope) : poolSize_(poolSize), gridWidth_(gridWidth), h_(poolSize/gridWidth), c2_(c*c), maxSlope_(maxSlope)
{
	int size = gridWidth_ * gridWidth_;
	/* Set up OpenCL */
	clUtil = OpenCLUtil();

	clUtil.printDeviceInfo(clUtil.getDevice());

	program = clUtil.createProgram("Kernels/Height_Field_Fluid_Sim.cl");

	/* Create kernel */
	kernel = cl::Kernel(*program, "ColumnSimStep");

	/* Create OpenCL buffers */
	clBuff_u = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, size * sizeof(cl_float));
	clBuff_u2 = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, size * sizeof(cl_float));
	v = cl::Buffer(clUtil.getContext(), CL_MEM_READ_WRITE, size * sizeof(float));

	// TODO: Initialise u with an interesting function

	/* Set kernel arguments */
	kernel.setArg(2, v);
	kernel.setArg(3, gridWidth_);
	kernel.setArg(4, h_);
	kernel.setArg(5, c2_);
	kernel.setArg(6, maxSlope_);
	/* Set up NDRanges */
	global = cl::NDRange(size);
	local = cl::NDRange(1);

	/* Create renderobject */
	shader = new Shader("BasicVert.glsl", "basicFrag.glsl");
	mesh = Mesh::GeneratePlane(poolSize_, gridWidth_);
	renderObject = RenderObject(mesh, shader);

	host_u = new cl_float[size];
	for (int i = 0; i < size; ++i) {
		host_u[i] = mesh->getVertices()[i].y;
	}

	for (int i = 0; i < gridWidth_; ++i) {
		host_u[i] = 5;
	}
	clUtil.getCommandQueue().enqueueWriteBuffer(clBuff_u, CL_TRUE, 0, size * sizeof(cl_float), host_u);
}


FluidSim::~FluidSim()
{
	delete program;
	delete shader;
	delete mesh;
	delete host_u;
}

void FluidSim::step(float dt)
{
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
	kernel.setArg(7, dt);

	/* Run kernel */
	auto err = clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	if (err != CL_SUCCESS)
	{
		std::cout << "Error running kernel (" << err << ")\n";
	}

	/* Wait for command queue to finish */
	clFinish(clUtil.getCommandQueue()());

	// Explicit copy via host because cl/gl interop is being an ass
	// Only copy the changed buffer
	
	if (flipBuff) {
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u2, CL_TRUE, 0, gridWidth_*gridWidth_ * sizeof(cl_float), host_u);
	}
	else {
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u, CL_TRUE, 0, gridWidth_*gridWidth_ * sizeof(cl_float), host_u);
	}
	clFinish(clUtil.getCommandQueue()());
	if (err != CL_SUCCESS)
	{
		std::cout << "Error reading buffers onto host (" << err << ")\n";
	}
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
	Vector3* ptr = (Vector3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < gridWidth_*gridWidth_; ++i) {
		ptr[i].y = host_u[i];
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	flipBuff = !flipBuff;
}
