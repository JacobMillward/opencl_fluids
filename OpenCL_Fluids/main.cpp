#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"


int main() {

	Window w = Window(800, 600);
	Renderer r(w);

	FluidSim fluid(100, 10, 0.015);
	r.AddRenderObject(fluid.getRenderObject());

	r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 95.0f));
	Vector3 cameraPosition = Vector3(45, 25, -50);
	Vector3 cameraLookAt = Vector3(44, 22, 2);
	float cameraSpeed = 5.0f;

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
		if (Keyboard::KeyTriggered(KEY_SPACE)) {
			printf("Position: %f, %f, %f\nLookat: %f, %f, %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraLookAt.x, cameraLookAt.y, cameraLookAt.z);
		}

		r.SetViewMatrix(Matrix4::BuildViewMatrix(cameraPosition,cameraLookAt));
		fluid.step(dt);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}

	return 0;
}
