#ifndef VAO_H
#define VAO_H
#include <glad.h>
#include <glfw3.h>
#include "typedefines.h"

struct vao{
	uint32 VAO,VBO,EBO;
};

// NOTE(Banni): Creates a new VAO.
void initialize_vao(vao* VAO);
// NOTE(Banni): Updates the existing VAO.
void update_vao(vao* VAO,
				float * Vertices,
				uint32 SizeOfVertices,
				uint32 * Indices,
				uint32 SizeOfIndices,
				uint32 Stride);

void use_vao(uint32 VAO);
#endif