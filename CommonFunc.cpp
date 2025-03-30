#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool init() {
    // Initialize SDL with video and audio support
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow(WINDOW_TITLE,
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void adjustSpeed(int& timeCounter, Uint32& lastTime, int& obstacleSpeed) {
    Uint32 currentTime = SDL_GetTicks();
    timeCounter += currentTime - lastTime;
    lastTime = currentTime;

    if (timeCounter >= SPEED_INCREMENT_INTERVAL) {
        obstacleSpeed += SPEED_INCREMENT;
        timeCounter = 0;
    }
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    IMG_Quit();
    SDL_Quit();
}
