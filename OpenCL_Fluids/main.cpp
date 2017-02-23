#pragma once
#include <iostream>
#include "OpenCLUtil.h"
#include <SDL.h>
#undef main

int main() {
	float poolSize = 100;

	cl_int gridWidth = 50;
	cl_float h = poolSize / gridWidth;
	cl_float c2 = 1; // TODO: Figure out what the hell this value is (?Wave propogation speed?)

	/* Lets hardcode the timestep for now until we integrate NCLGL */
	cl_float dt = 0.01f;

	/* Set up OpenCL */
	OpenCLUtil clUtil = OpenCLUtil();

	clUtil.printDeviceInfo();

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

	//Set up NDRanges
	cl::NDRange global(gridWidth * gridWidth);
	cl::NDRange local(1);

	/* Set up SDL */
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN); if (window == NULL) { printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()); }
	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );
	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	//Update the surface
	SDL_UpdateWindowSurface( window );
	
	//Main Loop
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		/* Handle SDL Events */
		while (SDL_PollEvent(&e) > 0) {
			switch (e.type) {
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
		/* Run our simulation step */
		clUtil.getCommandQueue().enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
	}
	//Destroy window
	SDL_DestroyWindow( window );
	SDL_Quit();
	delete program;
	return 0;
}
