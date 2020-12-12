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

	int GetVertexCount() const { return vertexVector.size(); }
	int GetIndexCount() const { return indexVector.size(); }

	void UploadMeshData() const;
	void Destroy() const;

	unsigned int GetVAO() const { return VAO; }
	unsigned int GetEBO() const { return EBO; }

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

#endif