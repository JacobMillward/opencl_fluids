#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"

float fluidSize = 100.0f;

int main() {

	Window w = Window(800, 600);
	Renderer r(w, fluidSize);

	FluidSim fluid(fluidSize, fluidSize, 0.005f, 20.0f, "test.jpg");
	r.AddRenderObject(fluid.getRenderObject());

	r.SetProjectionMatrix(Matrix4::Perspective(1, 800, 0.66f, 95.0f));
	Vector3 cameraPosition = Vector3(45, 25, -50);
	Vector3 cameraLookAt = Vector3(44, 22, 2);
	int timeScale = 8;
	bool timeWarpEnabled = false;

	while (w.UpdateWindow()) {
		float dt = w.GetTimer()->GetTimedMS();

		/* Timescale Controls */
		if (Keyboard::KeyTriggered(KEY_LEFT)) {
			timeScale = max(1, timeScale / 2);
			cout << timeScale << "x\n";
		}
		if (Keyboard::KeyTriggered(KEY_RIGHT)) {
			timeScale *= 2;
			cout << timeScale << "x\n";
		}
		if (Keyboard::KeyTriggered(KEY_T)) {
			timeWarpEnabled = !timeWarpEnabled;
			cout << "Timewarp: " << (timeWarpEnabled ? "Enabled " : "Disabled ") << timeScale << "x\n";
		}
		/* Step the fluid forward */
		if (timeWarpEnabled) {
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
