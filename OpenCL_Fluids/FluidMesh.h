#pragma once
#include "..\nclgl\Mesh.h"

class FluidMesh : public Mesh
{
public:
	FluidMesh();
	~FluidMesh();

	void setVertices(GLuint vbo);
};

