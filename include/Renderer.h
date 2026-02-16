#pragma once

#include <glad/glad.h>
#include "shaderClass.h"

// ============================================================
// Renderer
// Responsible for drawing 2D squares using pixel coordinates
// Converts pixel coordinates -> Normalized Device Coordinates
// ============================================================

class Renderer
{
public:
    // Constructor
    // screenWidth & screenHeight are used to convert pixel -> NDC
    Renderer(int screenWidth, int screenHeight);

    // Draw a square
    // x, y = position in pixels (top-left origin)
    // width, height = size in pixels
    // r,g,b = color (0.0 - 1.0)
    // textureID = optional (0 means no texture)
    void drawSquare(float x, float y,
                    float width, float height,
                    float r, float g, float b,
                    unsigned int textureID = 0);

    void setScreenSize(int width, int height);  

private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;

    int screenWidth;
    int screenHeight;

    void setupBuffers();
};