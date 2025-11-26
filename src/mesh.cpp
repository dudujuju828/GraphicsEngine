
#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include <spdlog/spdlog.h>

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

struct Vec3 {
	float x, y, z;
};

Mesh::Mesh(const std::vector<float>& positions) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(float),
                 positions.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertexCount = static_cast<int>(positions.size() / 3);
}

Mesh::Mesh(std::filesystem::path objfile_path) {
	std::ifstream infile(objfile_path);
	std::vector<float> v_pos;
	
	std::string	line;
	while (std::getline(infile, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "v") {
			Vec3 v;
			iss >> v.x >> v.y >> v.z;	
			v_pos.push_back(v.x);	
			v_pos.push_back(v.y);	
			v_pos.push_back(v.z);	
		}	
	}
	
	glGenBuffers(1,&VBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,v_pos.size()*sizeof(float),v_pos.data(),GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	
	vertexCount = static_cast<int>(v_pos.size() / 3);
}

void Mesh::use() {
	glBindVertexArray(VAO);
}

void Mesh::draw() {
	use();
	glDrawArrays(GL_TRIANGLES,0,vertexCount);
}
