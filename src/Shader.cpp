#include <shader.h>
#include <defensive.h>

#include <string>
#include "glm/gtc/type_ptr.inl"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
{
	_vertex_shader_path = std::string(vertex_path);
	_fragment_shader_path = std::string(fragment_path);
	_geometry_shader_path = std::string(geometry_path);
	_is_compiled = false;
}

bool Shader::Compile()
{
	if(_vertex_shader_path.empty())
	{
		std::cout << "ERROR::SHADER::VERTEX_SHADER_NOT_SPECIFIED" << std::endl;
		return false;
	}
	if(_fragment_shader_path.empty())
	{
		std::cout << "ERROR::SHADER::FRAGMENT_SHADER_NOT_SPECIFIED" << std::endl;
		return false;
	}

	std::stringstream shader_stream;
	std::ifstream shader_file;
	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// load vertex/fragment shaders
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;

	try {
		shader_stream.str(std::string());
		shader_file.open(_vertex_shader_path);
		shader_stream << shader_file.rdbuf();
		shader_file.close();

		vertex_code = shader_stream.str();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	try {
		shader_stream.str(std::string());
		shader_file.open(_fragment_shader_path);
		shader_stream << shader_file.rdbuf();
		shader_file.close();

		fragment_code = shader_stream.str();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	if(!_geometry_shader_path.empty())
	{
		try {
			shader_stream.str(std::string());
			shader_file.open(_geometry_shader_path);
			shader_stream << shader_file.rdbuf();
			shader_file.close();

			geometry_code = shader_stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
	}

	int success;
	char infoLog[1024];

	// compile shaders
	auto vertex_shader_code = vertex_code.c_str();
	auto fragment_shader_code = fragment_code.c_str();
	auto geometry_shader_code = geometry_code.c_str();

	const auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint geometry = 0;
	if(!geometry_code.empty())
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_shader_code, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	}

	// link program
	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, vertex);
	glAttachShader(ProgramId, fragment);
	ExecuteIf(geometry, glAttachShader(ProgramId, geometry));
	glLinkProgram(ProgramId);
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ProgramId, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK\n" << infoLog << std::endl;
	}

	// delete the shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::Use()
{
	glUseProgram(ProgramId);	
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ProgramId, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
	const auto location = glGetUniformLocation(ProgramId, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
