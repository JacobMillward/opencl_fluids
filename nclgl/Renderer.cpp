#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer(void)	{
	glDeleteTextures(1, &brickTex);
	glDeleteTextures(1, &staticTex);
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
		glUniform1f(glGetUniformLocation(program, "time"), time);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickTex);
		glUniform1i(glGetUniformLocation(program, "baseTex"), 0);

		if (currentShader == TYPE_BLEND) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, staticTex);
			glUniform1i(glGetUniformLocation(program, "staticTex"), 1);
		}		

		UpdateShaderMatrices(program);

		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	time += msec;
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}
}

GLuint Renderer::LoadTexture(string name)
{
	return SOIL_load_OGL_texture(name.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}