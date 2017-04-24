#pragma once
#include "../nclgl/Mesh.h"

class FluidMesh : public Mesh
{
public:
	FluidMesh(float gridSize, int verticesPerSide, int textureWidth);
	~FluidMesh();

private:
	int gridSize;
};
