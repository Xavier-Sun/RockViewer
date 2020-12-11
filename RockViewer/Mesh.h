#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	std::vector<Vertex> vertexVector;
	std::vector<unsigned int> indexVector;

	Mesh();

	int GetVertexCount() { return vertexVector.size(); }
	int GetIndexCount() { return indexVector.size(); }

	void UploadMeshData();
	void Destroy();

	unsigned int GetVAO() { return VAO; }
	unsigned int GetEBO() { return EBO; }

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

#endif