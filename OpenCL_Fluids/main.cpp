#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(100, 100, 0.004, 10);
	r.AddRenderObject(fluid.getRenderObject());

	r.SetProjectionMatrix(Matrix4::Perspective(1, 800, 1.33f, 95.0f));
	Vector3 cameraPosition = Vector3(45, 25, -50);
	Vector3 cameraLookAt = Vector3(44, 22, 2);
	int timeScale = 10;

	while (w.UpdateWindow()) {
		float dt = w.GetTimer()->GetTimedMS();

		/* Timescale Controls */
		if (Keyboard::KeyTriggered(KeyboardKeys::KEY_LEFT)) {
			timeScale = max(1, timeScale /= 10);
		}
		if (Keyboard::KeyTriggered(KeyboardKeys::KEY_RIGHT)) {
			timeScale *= 10;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_SPACE)) {
			fluid.step(dt*(1.0f/timeScale));
		}
		else {
			fluid.step(dt);
		}
		
		r.ClearBuffers();
		r.UpdateScene(dt);
		r.RenderScene();
		r.SwapBuffers();
	}

	return 0;
}
