#pragma once

#include <string>
#include <map>

#include "globals_ENGINE.h"

class Shader
{
	unsigned int _vertex_sh_id{0};
	unsigned int _fragment_sh_id{0};
	unsigned int _program_id{0};

	std::map<std::string, int> _uniformLocations;


public:
	inline unsigned int id() const { return _program_id; }

	Shader() = default;
	Shader(Shader&& b) noexcept;
	~Shader();
	
	void compile(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
	void unload();
	void bind();

	void setUniform(const std::string& uniform_name, const vec3& val);
	
private:
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

};

