#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(50, 50, 1);

	while (w.UpdateWindow()) {
		fluid.step(0.01f);
	}

	return 0;
}
