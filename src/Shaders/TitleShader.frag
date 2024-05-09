#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

uniform float time; // Time uniform for controlling the movement of the gradient
uniform float fadeInDuration; // Duration of fade-in animation

void main()
{
    // Define the gradient colors
    vec3 color1 = vec3(1.0, 0.0, 0.0);  // Red
    vec3 color2 = vec3(0.0, 0.0, 1.0);  // Blue

    // Calculate the gradient position based on time
    float t = fract(time * 0.2); // Smooth transition between red and blue
    float gradientPos;

    // Transition from red to blue
    if (time < 0.5) {
        gradientPos = smoothstep(0.0, 1.0, t);
    } 
    // Transition from blue to red
    else {
        t = 1.0 - t; // Invert the time value
        gradientPos = smoothstep(0.0, 1.0, t);
    }

    // Interpolate between the two colors based on the gradient position
    vec3 finalColor = mix(color1, color2, gradientPos);

    // Get the color from the texture
    vec4 textureColor = texture(texture1, TexCoord);

    // Calculate fade-in alpha based on elapsed time and fadeInDuration
    float alpha = clamp(time / fadeInDuration, 0.0, 1.0);

    // Mix the texture color with the gradient color and apply fade-in alpha
    FragColor = vec4(finalColor * textureColor.rgb, textureColor.a * alpha);
}
