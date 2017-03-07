#include "FluidMesh.h"



FluidMesh::FluidMesh(float gridSize, int verticesPerSide, int textureWidth) : gridSize(verticesPerSide*verticesPerSide), Mesh()
{
	/* Set up arrays */
	numVertices = verticesPerSide*verticesPerSide;
	numIndices = (verticesPerSide - 1)*(verticesPerSide - 1)*6;
	vertices = new Vector3[numVertices];
	colours = new Vector4[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];
	/* Generate vertices */
	float distanceBetweenPoints = gridSize / verticesPerSide;
	for (int z = 0; z < verticesPerSide; ++z) {
		for (int x = 0; x < verticesPerSide; ++x) {
			vertices[z * verticesPerSide + x] = Vector3(x*distanceBetweenPoints, 0.0f, z*distanceBetweenPoints);
			colours[z * verticesPerSide + x] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			textureCoords[z * verticesPerSide + x] = Vector2(x * (verticesPerSide/(float)textureWidth), z * (verticesPerSide/(float)textureWidth));
		}
	}
	/* Generate indices */
	numIndices = 0;
	for (int x = 0; x < verticesPerSide - 1; ++x) {
		for (int z = 0; z < verticesPerSide - 1; ++z) {
			int a = (x      * (verticesPerSide)) + z;
			int b = ((x + 1) * (verticesPerSide)) + z;
			int c = ((x + 1) * (verticesPerSide)) + (z + 1);
			int d = (x      * (verticesPerSide)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	BufferData();

}


FluidMesh::~FluidMesh()
{
}

