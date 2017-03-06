#include "Renderer.h"

Renderer::Renderer(Window &parent, float fluidSize) : OGLRenderer(parent)	{
	camera = new Camera(0.0f, 180.0f, Vector3(45, 25, -50));
	//Create our light
	light = new  Light(Vector3(fluidSize, 20.0f, fluidSize/2.0f),
		Vector4(1, 1, 1, 1),
		fluidSize * 2.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer(void)	{
	delete camera;
	delete light;
}

void	Renderer::RenderScene() {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	if(o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetShaderProgram();	
		glUseProgram(program);
		UpdateShaderMatrices(program);
		SetShaderLight(program, *light);
		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	/*for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}*/
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

GLuint Renderer::LoadTexture(string name)
{
	return SOIL_load_OGL_texture(name.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}