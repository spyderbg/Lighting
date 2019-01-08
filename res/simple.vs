#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex;

out vec3 vCol;
out vec2 vTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vCol = vec3(0.3f, 0.5f, 0.6f);
	vTex = tex;
}
