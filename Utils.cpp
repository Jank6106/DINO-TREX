#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "Utils.h"

#include <fstream>
#include <iostream>

// Initialize variables
Mix_Chunk* menuSound = nullptr;
Mix_Chunk* gameSound = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* loseSound = nullptr;
Mix_Chunk* clickSound = nullptr;
int currentSoundChannel = -1;

TTF_Font* font = nullptr;
SDL_Color textColor = {128, 0, 32, 255};
SDL_Color goldColor = {255, 215, 0, 255};

SDL_Texture* pauseButtonTexture = nullptr;
SDL_Texture* continueButtonTexture = nullptr;
SDL_Texture* replayButtonTexture = nullptr;
SDL_Texture* exitButtonTexture = nullptr;
SDL_Texture* homeButtonTexture = nullptr;
SDL_Texture* pauseHomeButtonTexture = nullptr;
SDL_Texture* gameOverImage = nullptr;
SDL_Texture* mainScreenTexture = nullptr;
SDL_Texture* playButtonTexture = nullptr;

const SDL_Rect pauseButtonRect = {SCREEN_WIDTH - 55, 5, 50, 50};
const SDL_Rect continueButtonRect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 300, 150};
const SDL_Rect replayButtonRect = {SCREEN_WIDTH/2 - 100 + 60, SCREEN_HEIGHT/2 + 70, 200, 100};
const SDL_Rect exitButtonRect = {SCREEN_WIDTH/2 - 100 - 50 - 100, SCREEN_HEIGHT/2 + 80, 200, 80};
const SDL_Rect homeButtonRect = {SCREEN_WIDTH/2 + 80 + 90, SCREEN_HEIGHT/2 + 75, 80, 80};
const SDL_Rect pauseHomeButtonRect = {SCREEN_WIDTH/2 + 70, SCREEN_HEIGHT/2 - 37, 78, 78};
const SDL_Rect gameOverRect = {SCREEN_WIDTH/2 - 500, SCREEN_HEIGHT/2 - 350, 1000, 500};
const SDL_Rect playButtonRect = {SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 300, 100};

// Sound functions
void playSoundEffect(Mix_Chunk* sound, int loops) {
    if (sound) {
        Mix_PlayChannel(-1, sound, loops);
    }
}

void stopCurrentSound() {
    if (currentSoundChannel != -1) {
        Mix_HaltChannel(currentSoundChannel);
        currentSoundChannel = -1;
    }
}

void playLoopingSound(Mix_Chunk* sound) {
    stopCurrentSound();
    if (sound) {
        currentSoundChannel = Mix_PlayChannel(-1, sound, -1);
    }
}

void loadSounds() {
    menuSound = Mix_LoadWAV("sounds/menu_sound.wav");
    gameSound = Mix_LoadWAV("sounds/play_sound.wav");
    jumpSound = Mix_LoadWAV("sounds/jump_sound.wav");
    loseSound = Mix_LoadWAV("sounds/lose_sound.wav");
    clickSound = Mix_LoadWAV("sounds/click_sound.wav");

    if (!menuSound || !gameSound || !jumpSound || !loseSound || !clickSound) {
        std::cerr << "Failed to load some audio files! Mix Error: " << Mix_GetError() << std::endl;
    }
}

void freeSounds() {
    stopCurrentSound();
    Mix_FreeChunk(menuSound);
    Mix_FreeChunk(gameSound);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(loseSound);
    Mix_FreeChunk(clickSound);
    menuSound = nullptr;
    gameSound = nullptr;
    jumpSound = nullptr;
    loseSound = nullptr;
    clickSound = nullptr;
}

// Font functions
bool loadFont() {
    font = TTF_OpenFont("VCOOPERB.ttf", 30);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void freeFont() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

SDL_Texture* renderText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    return textTexture;
}

// Button functions
bool loadTextures() {
    pauseButtonTexture = LoadTexture("imgs/Buttons/Pause.png", renderer);
    continueButtonTexture = LoadTexture("imgs/Buttons/ContinueButton.png", renderer);
    replayButtonTexture = LoadTexture("imgs/Buttons/ReplayButton.png", renderer);
    exitButtonTexture = LoadTexture("imgs/Buttons/ExitButton.png", renderer);
    homeButtonTexture = LoadTexture("imgs/Buttons/HomeButton.jpg", renderer);
    pauseHomeButtonTexture = LoadTexture("imgs/Buttons/HomeButton.jpg", renderer);
    gameOverImage = LoadTexture("imgs/Buttons/GameOver.png", renderer);
    mainScreenTexture = LoadTexture("imgs/Background/mainScreen.png", renderer);
    playButtonTexture = LoadTexture("imgs/Buttons/PlayButton.png", renderer);

    if (!pauseButtonTexture || !continueButtonTexture || !replayButtonTexture ||
        !exitButtonTexture || !homeButtonTexture || !pauseHomeButtonTexture ||
        !gameOverImage || !mainScreenTexture || !playButtonTexture) {
        std::cerr << "Failed to load some textures!" << std::endl;
        return false;
    }
    return true;
}

void freeTextures() {
    if (pauseButtonTexture) SDL_DestroyTexture(pauseButtonTexture);
    if (continueButtonTexture) SDL_DestroyTexture(continueButtonTexture);
    if (replayButtonTexture) SDL_DestroyTexture(replayButtonTexture);
    if (exitButtonTexture) SDL_DestroyTexture(exitButtonTexture);
    if (homeButtonTexture) SDL_DestroyTexture(homeButtonTexture);
    if (pauseHomeButtonTexture) SDL_DestroyTexture(pauseHomeButtonTexture);
    if (gameOverImage) SDL_DestroyTexture(gameOverImage);
    if (mainScreenTexture) SDL_DestroyTexture(mainScreenTexture);
    if (playButtonTexture) SDL_DestroyTexture(playButtonTexture);
}

void renderInGameScores(SDL_Renderer* renderer, int score, int highScore) {
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Texture* scoreTexture = renderText(scoreText, renderer, font, textColor);
    if (scoreTexture) {
        SDL_Rect scoreRect = {10, 0, 0, 0};
        SDL_QueryTexture(scoreTexture, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTexture);
    }

    std::string highScoreText = "Highest: " + std::to_string(highScore);
    SDL_Texture* highScoreTexture = renderText(highScoreText, renderer, font, textColor);
    if (highScoreTexture) {
        SDL_Rect highScoreRect = {10, 30, 0, 0};
        SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreRect.w, &highScoreRect.h);
        SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
        SDL_DestroyTexture(highScoreTexture);
    }
}

void renderGameOverScreen(SDL_Renderer* renderer, int score, int highScore) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    if (gameOverImage) {
        SDL_RenderCopy(renderer, gameOverImage, nullptr, &gameOverRect);
    }

    std::string scoreText = "Your Score: " + std::to_string(score);
    SDL_Texture* scoreTextTexture = renderText(scoreText, renderer, font, goldColor);
    if (scoreTextTexture) {
        SDL_Rect scoreRect = {0, SCREEN_HEIGHT/2 - 50, 0, 0};
        SDL_QueryTexture(scoreTextTexture, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
        scoreRect.x = SCREEN_WIDTH/2 - scoreRect.w/2;
        SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTextTexture);
    }

    std::string highScoreText = "Highest Score: " + std::to_string(highScore);
    SDL_Texture* highScoreTexture = renderText(highScoreText, renderer, font, goldColor);
    if (highScoreTexture) {
        SDL_Rect highScoreRect = {0, SCREEN_HEIGHT/2, 0, 0};
        SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreRect.w, &highScoreRect.h);
        highScoreRect.x = SCREEN_WIDTH/2 - highScoreRect.w/2;
        SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
        SDL_DestroyTexture(highScoreTexture);
    }

    SDL_RenderCopy(renderer, replayButtonTexture, nullptr, &replayButtonRect);
    SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButtonRect);
    SDL_RenderCopy(renderer, homeButtonTexture, nullptr, &homeButtonRect);
}

void renderPauseScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    if (continueButtonTexture) {
        SDL_RenderCopy(renderer, continueButtonTexture, nullptr, &continueButtonRect);
    }
    if (pauseHomeButtonTexture) {
        SDL_RenderCopy(renderer, pauseHomeButtonTexture, nullptr, &pauseHomeButtonRect);
    }
}

void renderMainScreen(SDL_Renderer* renderer) {
    if (mainScreenTexture) {
        SDL_RenderCopy(renderer, mainScreenTexture, nullptr, nullptr);
    }
    if (playButtonTexture) {
        SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButtonRect);
    }
}
