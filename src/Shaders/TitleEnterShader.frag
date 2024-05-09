#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

uniform float time; // Time uniform for controlling the fade-in animation
uniform float fadeInDuration; // Duration of fade-in animation

void main()
{
    // Get the color from the texture
    vec4 textureColor = texture(texture1, TexCoord);

    // Calculate fade-in alpha based on elapsed time and fadeInDuration
    float alpha = clamp(time / fadeInDuration, 0.0, 1.0);

    // Apply fade-in alpha to the texture color
    FragColor = vec4(textureColor.rgb, textureColor.a * alpha);
}
