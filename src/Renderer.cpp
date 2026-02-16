#include "Renderer.h"
#include <iostream>

// ===============================================
// Constructor
// Loads shader and sets up OpenGL buffers
// ===============================================
Renderer::Renderer(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Load shader files
    shader = new Shader("assets/shaders/default.vert",
                        "assets/shaders/default.frag");

    // Create VAO, VBO, EBO
    setupBuffers();

    // Tell shader that texture1 uses texture unit 0
    shader->Activate();
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
}

// ===============================================
// Setup square geometry buffers
// ===============================================
void Renderer::setupBuffers()
{
    // Vertex format:
    // position (x,y,z) + texture coordinates (u,v)
    float vertices[] = {
        // x, y, z      u, v
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// ===============================================
// Draw square in pixel coordinates
// ===============================================
void Renderer::drawSquare(float x, float y, float width, float height,
                          float r, float g, float b,
                          unsigned int textureID)
{
    shader->Activate();

    // Convert pixel coordinates to NDC (-1 to 1)
    float left   = (2.0f * x / screenWidth) - 1.0f;
    float right  = (2.0f * (x + width) / screenWidth) - 1.0f;
    float top    = 1.0f - (2.0f * y / screenHeight);
    float bottom = 1.0f - (2.0f * (y + height) / screenHeight);

    // Update square position dynamically
    float vertices[] = {
        left,  bottom, 0.0f,  0.0f, 0.0f,
        right, bottom, 0.0f,  1.0f, 0.0f,
        right, top,    0.0f,  1.0f, 1.0f,
        left,  top,    0.0f,  0.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(vertices), vertices);

    glBindVertexArray(VAO);

    // Send color to shader
    glUniform3f(glGetUniformLocation(shader->ID, "uColor"),
                r, g, b);

    // If texture exists
    if (textureID != 0)
    {
        glUniform1i(glGetUniformLocation(shader->ID, "useTexture"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader->ID, "useTexture"), 0);
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::setScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}