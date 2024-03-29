/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related 
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents. 


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "OGLRenderer.h"
#include <vector>

using std::ifstream;
using std::string;

//A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer
{
	VERTEX_BUFFER =0,
	COLOUR_BUFFER =1,
	TEXTURE_BUFFER,
	NORMAL_BUFFER,
	TANGENT_BUFFER,
	INDEX_BUFFER,
	MAX_BUFFER
};

class Mesh
{
public:
	friend class MD5Mesh;
	Mesh(void);
	virtual ~Mesh(void);

	virtual void Draw();

	//Generates a single triangle, with RGB colours
	static Mesh* GenerateTriangle();
	//Generates an X by X plane of points
	static Mesh* GeneratePlane(float sideLength, int pointsPerSide);
	static Mesh* LoadMeshFile(const string& filename);

	Vector3* getVertices() const { return vertices; };
	GLuint getVertexBuffer() { return bufferObject[VERTEX_BUFFER]; };
	Vector3* getNormals() const { return normals; };
	GLuint getNormalBuffer() { return bufferObject[NORMAL_BUFFER]; };
	//Generates normals for the vertexes
	void GenerateNormals();
	GLuint type; //Primitive type for this mesh (GL_TRIANGLES...etc)

protected:
	//Buffers all VBO data into graphics memory. Required before drawing!
	void BufferData();

	//VAO for this mesh
	GLuint arrayObject;
	//VBOs for this mesh
	GLuint bufferObject[MAX_BUFFER];
	//Number of vertices for this mesh
	GLuint numVertices;

	//Number of indices for this mesh
	GLuint numIndices;

	//Pointer to vertex position attribute data (badly named...?)
	Vector3* vertices;
	//Pointer to vertex colour attribute data
	Vector4* colours;
	//Pointer to vertex texture coordinate attribute data
	Vector2* textureCoords;
	//Pointer to vertex normals attribute data
	Vector3* normals;
	//Pointer to vertex tangents attribute data
	Vector3* tangents;
	//Pointer to vertex indices attribute data
	unsigned int* indices;
};
