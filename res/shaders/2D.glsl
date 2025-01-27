#type VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = vec4(aPos, 3.0);
	TexCoord = aTexCoord;
}

#type FRAGMENT
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1,TexCoord);
}
