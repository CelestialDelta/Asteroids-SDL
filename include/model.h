#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

class Model
{
public:
	Model(const std::string& file = std::string());
	
	bool load(const std::string& file);
	void unload();

	bool is_loaded() const;
	std::vector<glm::vec2> vertices() const;
private:
	std::string _file;
	std::vector<glm::vec2> _vertices;
};
