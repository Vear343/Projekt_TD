#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

#include "RenderWindow.h"
#include "Entity.h"

// Constructor to create a window and renderer
RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
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
    SDL_RenderClear(renderer);
}

// Render a texture to the window
void RenderWindow::render(Entity& p_entity)
{
    // Define the source rectangle for rendering the texture
    SDL_Rect src;
    // Starting point (top-left corner) of the texture
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    // texture pixels width and height
    src.w = p_entity.getCurrentFrame().w; 
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dst;
    // position on the window
    dst.x = p_entity.getX() * 4; 
    dst.y = p_entity.getY() * 4;
    // size of the rendered texture on the window
    dst.w = p_entity.getCurrentFrame().w * 4;
    dst.h = p_entity.getCurrentFrame().h * 4;

    SDL_RenderCopy(renderer, p_entity.getTexture(), &src, &dst);

}

// Present the rendered content to the screen
void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}