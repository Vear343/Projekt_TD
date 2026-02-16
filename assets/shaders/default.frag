#version 330 core

// =======================================
// OUTPUT COLOR
// =======================================

out vec4 FragColor;

// =======================================
// INPUT FROM VERTEX SHADER
// =======================================

in vec2 TexCoord;

// =======================================
// UNIFORMS (Set from C++)
// =======================================

// Color for non-textured squares
uniform vec3 uColor;

// Texture sampler
uniform sampler2D texture1;

// Whether to use texture or not
uniform bool useTexture;

// =======================================
// MAIN
// =======================================
void main()
{
    // If we want to use texture
    if (useTexture)
    {
        // Sample texture color
        FragColor = texture(texture1, TexCoord);
    }
    else
    {
        // Use plain color
        FragColor = vec4(uColor, 1.0);
    }
}