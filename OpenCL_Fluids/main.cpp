#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(4, 2, 1);
	//r.AddRenderObject(fluid.getRenderObject());

	//while (w.UpdateWindow()) {
		fluid.step(0.01f);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	//}

	return 0;
}
