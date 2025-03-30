#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);

#endif
