#version 330 core

// =======================================
// INPUTS FROM VBO
// =======================================

// Location 0 → vertex position (x, y, z)
layout (location = 0) in vec3 aPos;

// Location 1 → texture coordinates (u, v)
layout (location = 1) in vec2 aTex;

// =======================================
// OUTPUT TO FRAGMENT SHADER
// =======================================

// Pass texture coordinates to fragment shader
out vec2 TexCoord;

// =======================================
// MAIN
// =======================================
void main()
{
    // Set final vertex position
    // aPos is already converted to NDC (-1 to 1)
    gl_Position = vec4(aPos, 1.0);

    // Pass texture coordinate to fragment shader
    TexCoord = aTex;
}