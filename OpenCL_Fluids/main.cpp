#pragma once
#include "../nclgl/Renderer.h"
#include "FluidSim.h"

float fluidSize = 1000.0f;

int main()
{
	auto w = Window(800, 600);
	Renderer r(w, fluidSize);

	FluidSim fluid(fluidSize, fluidSize/2, 0.004f, 3.0f, CL_DEVICE_TYPE_CPU);
	r.AddRenderObject(fluid.getFluidRenderObject());
	r.AddRenderObject(fluid.getCubeRenderObject());
	r.SetProjectionMatrix(Matrix4::Perspective(1, 800, 800.0f / 600.0f, 95.0f));
	auto cameraPosition = Vector3(45, 25, -50);
	auto cameraLookAt = Vector3(44, 22, 2);
	auto timeScale = 8;
	auto timeWarpEnabled = false;

	TimeDifference averageExecutionTime{};
	auto framecount = 0;
	while (w.UpdateWindow())
	{
		auto dt = w.GetTimer()->GetTimedMS();

		/* Timescale Controls */
		if (Keyboard::KeyTriggered(KEY_LEFT))
		{
			timeScale = max(1, timeScale / 2);
			std::cout << timeScale << "x\n";
		}
		if (Keyboard::KeyTriggered(KEY_RIGHT))
		{
			timeScale *= 2;
		std::cout << timeScale << "x\n";
		}
		if (Keyboard::KeyTriggered(KEY_T))
		{
			timeWarpEnabled = !timeWarpEnabled;
			std::cout << "Timewarp: " << (timeWarpEnabled ? "Enabled " : "Disabled ") << timeScale << "x\n";
		}
		/* Step the fluid forward */
		if (timeWarpEnabled)
		{
			fluid.step(dt * (1.0f / timeScale));
		}
		else
		{
			fluid.step(dt);
		}

		averageExecutionTime += fluid.getKernelExecutionTime();
		std::cout << chrono::duration<double, milli>(fluid.getKernelExecutionTime()).count() << " ms, " << chrono::duration<double, milli>(fluid.getBufferCopyExecutionTime()).count() << " ms" << endl;

		r.ClearBuffers();
		r.UpdateScene(dt);
		r.RenderScene();
		r.SwapBuffers();
	}

	return 0;
}
