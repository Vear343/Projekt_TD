#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

#include "RenderWindow.h"
#include "Entity.h"

// Constructor to create a window and renderer
RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL),
     renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    // Check if the window was created successfully 
    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}

// Load a texture from a file path
SDL_Texture* RenderWindow::loadTexture(const char* p_filepPath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filepPath);
    // Check if the texture was loaded successfully
    if (texture == NULL)
    {
        std::cout << "Unable to load texture! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

// Clean up resources
void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

// Clear the renderer
void RenderWindow::clear()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Background Color
    SDL_RenderClear(renderer);
}

// draw a full-screen texture (e.g. sky image)
void RenderWindow::drawTextureFull(SDL_Texture* tex)
{
    if (!tex) return;
    SDL_Rect dest = { 0, 0, getwidth(), getheight() };
    SDL_RenderCopy(renderer, tex, NULL, &dest);
}

// Render a texture to the window at entity size
void RenderWindow::render(Entity& p_entity)
{
    SDL_Texture* texture = p_entity.getTexture();

    // If entity has no texture, skip rendering
    if (!texture) return;

    // Query actual texture dimensions for source rectangle
    int texWidth, texHeight;
    SDL_QueryTexture(texture, NULL, NULL, &texWidth, &texHeight);

    // Define the source rectangle (use full texture)
    SDL_Rect src = { 0, 0, texWidth, texHeight };

    // Define the destination rectangle (use entity's specified size)
    SDL_Rect dst;
    dst.x = (int)(p_entity.getX() - (p_entity.getWidth() / 2.0f));  // Center on x-axis
    dst.y = (int)(p_entity.getY() - (p_entity.getHeight() / 2.0f)); // Center on y-axis
    dst.w = (int)p_entity.getWidth();   // Use entity's width
    dst.h = (int)p_entity.getHeight();  // Use entity's height

    SDL_RenderCopy(renderer, texture, &src, &dst);
    
    if (drawColliders) {
        // Draw collider for debugging
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect coll = p_entity.getCollider();
        SDL_RenderDrawRect(renderer, &coll);
    }
}

// Present the rendered content to the screen
void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

int RenderWindow::getwidth() {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return w;
}
int RenderWindow::getheight() {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return h;
}