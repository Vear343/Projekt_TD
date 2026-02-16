#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Texture.h"

// Window size
int currentWidth = 800;
int currentHeight = 600;

// Handle window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

	currentWidth = width;
    currentHeight = height;
}

int main()
{
    // ===============================
    // Initialize GLFW
    // ===============================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(currentWidth, currentHeight, "2D Renderer", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ===============================
    // Load GLAD
    // ===============================
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, currentWidth, currentHeight);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create renderer
    Renderer renderer(currentWidth, currentHeight);

    // Load texture
    Texture catTex("assets/textures/pop_cat.png",
                   GL_TEXTURE_2D,
                   GL_TEXTURE0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE);

    // ===============================
    // Render loop
    // ===============================
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Update renderer with new size
		renderer.setScreenSize(width, height);

		float squareSize = std::min(width, height);
		float center_x = (width - squareSize) / 2.0f;
		float center_y = (height - squareSize) / 2.0f;

		renderer.drawSquare(center_x, center_y, width, height, 1.0f, 0.0f, 0.0f);
        renderer.drawSquare(400, 200, 512, 512, 1.0f, 1.0f, 1.0f, catTex.ID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}