#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include "Entity.h"

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h); 
    // Load a texture from a file path
    SDL_Texture* loadTexture(const char* p_filePath);
    void cleanUp();
    void clear();
    void render(Entity& p_entity);
    void display();
    int getwidth();
    int getheight();
    
    // draw a texture stretched to fill the entire window
    void drawTextureFull(SDL_Texture* tex);
    SDL_Window* getSDLWindow() { return window; }
    SDL_Renderer* getRenderer() {
        return renderer; // return the renderer so it can be used in other classes (like Level)
    }

    // enable/disable drawing colliders for debugging
    void setDrawColliders(bool enable) { drawColliders = enable; }
    bool isDrawColliders() const { return drawColliders; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool drawColliders = false; // default off
};