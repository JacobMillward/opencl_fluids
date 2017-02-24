#pragma once
#include "../nclgl/OGLRenderer.h"

#include "RenderObject.h"

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
	enum ShaderTypes { TYPE_SHRINK, TYPE_FADE, TYPE_BLEND, TYPE_CUBE };
	ShaderTypes currentShader = TYPE_SHRINK;
	void	resetTime() { time = 0; }
protected:
	GLuint LoadTexture(string name);
	vector<RenderObject*> renderObjects;
	GLuint brickTex;
	GLuint staticTex;
	float time = 0;
};

