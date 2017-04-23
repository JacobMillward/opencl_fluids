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
	auto distanceBetweenPoints = gridSize / verticesPerSide;
	for (auto z = 0; z < verticesPerSide; ++z) {
		for (auto x = 0; x < verticesPerSide; ++x) {
			vertices[z * verticesPerSide + x] = Vector3(x*distanceBetweenPoints, 0.0f, z*distanceBetweenPoints);
			colours[z * verticesPerSide + x] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			textureCoords[z * verticesPerSide + x] = Vector2((x/8.0f) * (verticesPerSide/static_cast<float>(textureWidth)), (z/8.0f) * (verticesPerSide/static_cast<float>(textureWidth)));
		}
	}
	/* Generate indices */
	numIndices = 0;
	for (auto x = 0; x < verticesPerSide - 1; ++x) {
		for (auto z = 0; z < verticesPerSide - 1; ++z) {
			auto a = (x      * (verticesPerSide)) + z;
			auto b = ((x + 1) * (verticesPerSide)) + z;
			auto c = ((x + 1) * (verticesPerSide)) + (z + 1);
			auto d = (x      * (verticesPerSide)) + (z + 1);

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

