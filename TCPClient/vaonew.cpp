#include "vaonew.h"

void initialize_vao(vao* VAO,
					float * Vertices,
					uint32 SizeOfVertices,
					uint32 * Indices,
					uint32 SizeOfIndices,
					uint32 Stride){
	glGenVertexArrays(1,&VAO->VAO);
	glBindVertexArray(VAO->VAO);
	
	glGenBuffers(1, &VAO->VBO);
	glGenBuffers(1, &VAO->EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VAO->VBO);
	glBufferData(GL_ARRAY_BUFFER,
				 SizeOfVertices,
				 Vertices,
				 GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 SizeOfIndices,
				 Indices,
				 GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  Stride, (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1,
						  2,
						  GL_FLOAT,
						  GL_FALSE,
						  Stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void use_vao(uint32 VAO){
	glBindVertexArray(VAO);
}