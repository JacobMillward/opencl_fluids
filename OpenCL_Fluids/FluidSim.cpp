#include "GL/glew.h"
#include "FluidSim.h"
#include <iostream>
#include <chrono>

FluidSim::FluidSim(float poolSize, int gridWidth, float c, float maxSlope, cl_device_type deviceType, string texturePath) : poolSize_(poolSize), gridWidth_(gridWidth), h_(poolSize / gridWidth), c2_(c * c), maxSlope_(maxSlope), clUtil(deviceType)
{
	auto size = gridWidth_ * gridWidth_;
	/* Set up OpenCL */
	//clUtil = OpenCLUtil(deviceType);

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

	/* Create fluid mesh */
	fluidShader = new Shader("BasicVert.glsl", "PhongLightingFrag.glsl");
	fluidMesh = new FluidMesh(poolSize_, gridWidth_, 256);
	fluidTexture = SOIL_load_OGL_texture(texturePath.c_str(), SOIL_LOAD_AUTO,
	                                     SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	fluidRenderObject = RenderObject(fluidMesh, fluidShader);
	fluidRenderObject.SetTexture(fluidTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fluidTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* Create cube */
	cubeMesh = Mesh::LoadMeshFile("cube.asciimesh");
	cubeRenderObject = RenderObject(cubeMesh, fluidShader);

	/* Set up host buffer */
	host_u = new cl_float[size];
	for (auto i = 0; i < size; ++i)
	{
		host_u[i] = fluidMesh->getVertices()[i].y;
	}

	/* Create wave in middle of fluid */
	auto center = size / 2 + gridWidth_ / 2;
	host_u[center] = 15;
	clUtil.getCommandQueue().enqueueWriteBuffer(clBuff_u, CL_TRUE, 0, size * sizeof(cl_float), host_u);
}


FluidSim::~FluidSim()
{
	delete program;
	delete fluidShader;
	delete fluidMesh;
	delete cubeMesh;
	delete host_u;
}

void FluidSim::step(float dt)
{
	auto startTime = chrono::steady_clock::now();

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
		cout << "Error running kernel (" << err << ")\n";
	}

	/* Wait for command queue to finish */
	clFinish(clUtil.getCommandQueue()());

	auto kernelEndTime = chrono::steady_clock::now();

	// Explicit copy via host because cl/gl interop is being an ass
	// Only copy the changed buffer

	if (flipBuff)
	{
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u2, CL_TRUE, 0, gridWidth_ * gridWidth_ * sizeof(cl_float), host_u);
	}
	else
	{
		err = clUtil.getCommandQueue().enqueueReadBuffer(clBuff_u, CL_TRUE, 0, gridWidth_ * gridWidth_ * sizeof(cl_float), host_u);
	}
	clFinish(clUtil.getCommandQueue()());
	if (err != CL_SUCCESS)
	{
		cout << "Error reading buffers onto host (" << err << ")\n";
	}
	glBindBuffer(GL_ARRAY_BUFFER, fluidMesh->getVertexBuffer());
	auto ptr = static_cast<Vector3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (auto i = 0; i < gridWidth_ * gridWidth_; ++i)
	{
		ptr[i].y = host_u[i];
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	fluidMesh->GenerateNormals();

	glBindBuffer(GL_ARRAY_BUFFER, fluidMesh->getNormalBuffer());
	glBufferData(GL_ARRAY_BUFFER, gridWidth_ * gridWidth_ * sizeof(Vector3), fluidMesh->getNormals(), GL_DYNAMIC_DRAW);
	flipBuff = !flipBuff;

	auto endTime = chrono::steady_clock::now();
	kernelExecutionTime = kernelEndTime - startTime;
	bufferCopyExecutionTime = endTime - kernelEndTime;
}

TimeDifference FluidSim::getKernelExecutionTime() const
{
	return kernelExecutionTime;
}

TimeDifference FluidSim::getBufferCopyExecutionTime() const
{
	return bufferCopyExecutionTime;
}
