#pragma once
#include "OGLRenderer.h"
#include "RenderObject.h"
#include "Camera.h"
#include <vector>

using std::vector;

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	~Renderer(void);

	virtual void	RenderScene();

	virtual void	Render(const RenderObject &o);

	virtual void	UpdateScene(float msec);

	void	AddRenderObject(RenderObject &r) {
		renderObjects.push_back(&r);
	}
	
	HDC& getDeviceContext() { return deviceContext; }
	HGLRC& getRenderContext() { return renderContext; }
protected:
	GLuint LoadTexture(string name);
	vector<RenderObject*> renderObjects;
	Camera* camera;
};

