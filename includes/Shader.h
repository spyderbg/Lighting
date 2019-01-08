#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ProgramId;

	Shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
	
	// compile shader 
	bool Compile();

	// use/activate the shader
	void Use();

	// utility uniform functions
	void SetBool(const std::string& name, bool value)const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat);

private:
	std::string _vertex_shader_path;
	std::string _fragment_shader_path;
	std::string _geometry_shader_path;

	bool _is_compiled;
};