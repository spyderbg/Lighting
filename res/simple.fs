#version 330

in vec3 vCol;
in vec2 vTex;
out vec4 gl_FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix_factor;

void main()
{
    gl_FragColor = mix(texture(texture1, vTex), texture(texture2, vec2(vTex.x, vTex.y)), mix_factor);
}
