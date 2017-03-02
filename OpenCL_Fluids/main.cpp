#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(100, 50, 0.015, 10);
	r.AddRenderObject(fluid.getRenderObject());

	r.SetProjectionMatrix(Matrix4::Perspective(1, 800, 1.33f, 95.0f));
	Vector3 cameraPosition = Vector3(45, 25, -50);
	Vector3 cameraLookAt = Vector3(44, 22, 2);
	float cameraSpeed = 10.0f;
	float timeScale = 10.0f;

	while (w.UpdateWindow()) {
		float dt = w.GetTimer()->GetTimedMS();
		float move = cameraSpeed * (dt / 1000);

		/* Camera Controls */
		if (Keyboard::KeyDown(KeyboardKeys::KEY_W)) {
			cameraPosition.z += move;
			cameraLookAt.z += move;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_A)) {
			cameraPosition.x += move;
			cameraLookAt.x += move;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_S)) {
			cameraPosition.z -= move;
			cameraLookAt.z -= move;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_D)) {
			cameraPosition.x -= move;
			cameraLookAt.x -= move;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_Q)) {
			cameraPosition.y += move;
			cameraLookAt.y += move;
		}
		if (Keyboard::KeyDown(KeyboardKeys::KEY_E)) {
			cameraPosition.y -= move;
			cameraLookAt.y -= move;
		}
		r.SetViewMatrix(Matrix4::BuildViewMatrix(cameraPosition, cameraLookAt));

		if (Keyboard::KeyDown(KeyboardKeys::KEY_SPACE)) {
			fluid.step(dt*(1/timeScale));
		}
		else {
			fluid.step(dt);
		}
		
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}

	return 0;
}
