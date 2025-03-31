#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "LTexture.h"

extern Mix_Chunk* menuSound;
extern Mix_Chunk* gameSound;
extern Mix_Chunk* jumpSound;
extern Mix_Chunk* loseSound;
extern Mix_Chunk* clickSound;
extern int currentSoundChannel;

extern TTF_Font* font;
extern SDL_Color textColor;
extern SDL_Color goldColor;

extern SDL_Texture* mainScreenTexture;
extern SDL_Texture* instructionTexture;
extern SDL_Texture* pauseButtonTexture;
extern SDL_Texture* pauseScreenTexture;
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* endScreenTexture;
extern SDL_Texture* settingsTexture; // Thêm texture cho Settings

extern int volume; // Biến toàn cục để lưu âm lượng

extern const SDL_Rect startButtonRect;
extern const SDL_Rect helpButtonRect;
extern const SDL_Rect exitButtonRect;
extern const SDL_Rect OptionsRect; // Đảm bảo khai báo OptionsRect
extern const SDL_Rect pauseButtonRect;
extern const SDL_Rect continueButtonRect;
extern const SDL_Rect homeButtonRect;
extern const SDL_Rect gameOverRect;
extern const SDL_Rect endScreenRect;
extern const SDL_Rect replayButtonRect;
extern const SDL_Rect gameOverHomeButtonRect;
extern const SDL_Rect backButtonRect;
extern const SDL_Rect decreaseButtonRect; // Nút giảm âm lượng
extern const SDL_Rect increaseButtonRect; // Nút tăng âm lượng

void playSoundEffect(Mix_Chunk* sound, int loops = 0);
void stopCurrentSound();
void playLoopingSound(Mix_Chunk* sound);
void loadSounds();
void freeSounds();

bool loadFont();
void freeFont();
SDL_Texture* renderText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

bool loadTextures();
void freeTextures();
void renderMainScreen(SDL_Renderer* renderer);
void renderHelpScreen(SDL_Renderer* renderer);
void renderPauseScreen(SDL_Renderer* renderer);
void renderSettingsScreen(SDL_Renderer* renderer); // Hàm vẽ màn hình Settings
void adjustVolume(int change); // Hàm điều chỉnh âm lượng
void renderInGameScores(SDL_Renderer* renderer, int score, int highScore);
void renderGameOverScreen(SDL_Renderer* renderer, int score, int highScore);

#endif

