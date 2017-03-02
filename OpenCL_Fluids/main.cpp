#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(4, 10, 1);
	r.AddRenderObject(fluid.getRenderObject());

	r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 45.0f));
	Matrix4 view = Matrix4::BuildViewMatrix(Vector3(0, 5, -5), Vector3(0, 0, 0));
	r.SetViewMatrix(view);

	while (w.UpdateWindow()) {
		//We need dt in seconds so times by 1000
		fluid.step(w.GetTimer()->GetTimedMS()*1000);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}

	return 0;
}
