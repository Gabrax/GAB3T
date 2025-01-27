#type VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;      // Vertex position
layout (location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoord; // Pass texture coordinates to fragment shader

uniform mat4 projection; // Projection matrix
uniform mat4 view;       // View matrix
uniform mat4 model;      // Model matrix

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}


#type FRAGMENT
#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Interpolated texture coordinates from vertex shader

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}

