#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	std::vector<Vertex> vertexVector;
	std::vector<unsigned int> indexVector;

	Mesh();
	~Mesh() = default;

	void Create();
	void Upload();
	void Destroy();

	size_t GetVertexCount() const { return vertexVector.size(); }
	size_t GetIndexCount() const { return indexVector.size(); }

	unsigned int GetVAO() const { return VAO; }
	unsigned int GetEBO() const { return EBO; }

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

#endif