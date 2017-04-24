#pragma once
#include "OGLRenderer.h"
#include "RenderObject.h"
#include "Camera.h"
#include <vector>

using std::vector;

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent, float fluidSize);
	~Renderer(void);

	void RenderScene() override;

	virtual void Render(const RenderObject& o);

	void UpdateScene(float msec) override;

	void AddRenderObject(RenderObject& r)
	{
		renderObjects.push_back(&r);
	}

	HDC& getDeviceContext() { return deviceContext; }
	HGLRC& getRenderContext() { return renderContext; }
protected:
	static GLuint LoadTexture(string name);
	vector<RenderObject*> renderObjects;
	Camera* camera;
	Light* light;
};
