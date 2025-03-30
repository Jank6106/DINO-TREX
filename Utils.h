#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"

// Sound system
extern Mix_Chunk* menuSound;
extern Mix_Chunk* gameSound;
extern Mix_Chunk* jumpSound;
extern Mix_Chunk* loseSound;
extern Mix_Chunk* clickSound;
extern int currentSoundChannel;

// Font system
extern TTF_Font* font;
extern SDL_Color textColor;
extern SDL_Color goldColor;

// Button textures
extern SDL_Texture* pauseButtonTexture;
extern SDL_Texture* continueButtonTexture;
extern SDL_Texture* replayButtonTexture;
extern SDL_Texture* exitButtonTexture;
extern SDL_Texture* homeButtonTexture;
extern SDL_Texture* pauseHomeButtonTexture;
extern SDL_Texture* gameOverImage;
extern SDL_Texture* mainScreenTexture;
extern SDL_Texture* playButtonTexture;

// Button positions
extern const SDL_Rect pauseButtonRect;
extern const SDL_Rect continueButtonRect;
extern const SDL_Rect replayButtonRect;
extern const SDL_Rect exitButtonRect;
extern const SDL_Rect homeButtonRect;
extern const SDL_Rect pauseHomeButtonRect;
extern const SDL_Rect gameOverRect;
extern const SDL_Rect playButtonRect;

// Sound functions
void playSoundEffect(Mix_Chunk* sound, int loops = 0);
void stopCurrentSound();
void playLoopingSound(Mix_Chunk* sound);
void loadSounds();
void freeSounds();

// Font functions
bool loadFont();
void freeFont();
SDL_Texture* renderText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

// Button functions
bool loadTextures();
void freeTextures();
void renderInGameScores(SDL_Renderer* renderer, int score, int highScore);
void renderGameOverScreen(SDL_Renderer* renderer, int score, int highScore);
void renderPauseScreen(SDL_Renderer* renderer);
void renderMainScreen(SDL_Renderer* renderer);

#endif // UTILS_H
