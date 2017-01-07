#include "../include/model.h"

Model::Model(const std::string& file)
	: _file(), _vertices()
{
	load(file);
}

bool Model::load(const std::string& file)
{
	std::ifstream fs(file);
	if(fs.is_open())
	{	
		std::string text;
		glm::vec2 v;
		
		bool x_found = false;
		bool y_found = false;
		while(fs)
		{
			//fs >> text;
			std::getline(fs, text);
			if(!text.empty())
			{
				//std::cout << text << std::endl;
				std::string::size_type p = text.find('_');
				std::string::size_type pp = text.find('_', p);
				
				std::string num = text.substr(p + 1, pp + 1);
				float n = std::stof(num);
				if(text.at(0) == 'x' && !x_found)
				{
					v.x = n;
					x_found = true;
				}
				else if(text.at(0) == 'y' && !y_found)
				{
					v.y = n;
					y_found = true;
				}
				
				if(x_found && y_found)
				{
					_vertices.push_back(v);
					
					x_found = false;
					y_found = false;
				}
			}
		}
		//std::cout << _vertices.size() << std::endl;
		return true;
	}
	return false;
}

void Model::unload()
{
	_file.clear();
	_vertices.clear();
}

bool Model::is_loaded() const
{
	return !_vertices.empty();
}

std::vector<glm::vec2> Model::vertices() const
{
	return _vertices;
}
