#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Graphics.h"
#include "Vertex.h"

class Mesh : public Graphics
{
public:
	void Upload(const std::vector<Vertex>& vertexVector, const std::vector<unsigned int>& indexVector);

	unsigned int GetVBO() const { return VBO; }
	unsigned int GetVAO() const { return VAO; }
	unsigned int GetEBO() const { return EBO; }

	unsigned int GetVertexNum() const { return vertexNum; }
	unsigned int GetIndexNum() const { return indexNum; }

	Mesh();
	~Mesh();

private:
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;

	unsigned int vertexNum = 0;
	unsigned int indexNum = 0;
};

#endif