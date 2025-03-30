#include "LTexture.h"
#include <iostream>

SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (!loadSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
//    SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 255, 255, 255));

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return newTexture;
}


