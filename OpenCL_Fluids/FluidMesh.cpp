#include "FluidMesh.h"



FluidMesh::FluidMesh() : Mesh()
{
}


FluidMesh::~FluidMesh()
{
}

void FluidMesh::setVertices(GLuint vbo)
{
	glBindVertexArray(arrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
}
