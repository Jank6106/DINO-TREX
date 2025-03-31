#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "Utils.h"

Mix_Chunk* menuSound = nullptr;
Mix_Chunk* gameSound = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* loseSound = nullptr;
Mix_Chunk* clickSound = nullptr;
int currentSoundChannel = -1;

TTF_Font* font = nullptr;
SDL_Color textColor = {128, 0, 32, 255};
SDL_Color goldColor = {255, 215, 0, 255};

SDL_Texture* mainScreenTexture = nullptr;
SDL_Texture* instructionTexture = nullptr;
SDL_Texture* pauseButtonTexture = nullptr;
SDL_Texture* pauseScreenTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* endScreenTexture = nullptr;
SDL_Texture* settingsTexture = nullptr; // Thêm texture cho Settings

int volume = 70; // Âm lượng mặc định là 70

const SDL_Rect startButtonRect = {620, 230, 240, 70};
const SDL_Rect helpButtonRect = {620, 480, 240, 70};
const SDL_Rect exitButtonRect = {620, 400, 240, 70};
const SDL_Rect OptionsRect = {620, 315, 240, 70};
const SDL_Rect pauseButtonRect = {SCREEN_WIDTH - 50, 10, 40, 40};
const SDL_Rect continueButtonRect = {320, 186, 356, 90};
const SDL_Rect homeButtonRect = {320, 320, 356, 90};
const SDL_Rect gameOverRect = {SCREEN_WIDTH/2 - 500, SCREEN_HEIGHT/2 - 480, 1000, 700};
const SDL_Rect endScreenRect = {SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT - 180, 500, 72};
const SDL_Rect replayButtonRect = {256, 413, 229, 58};
const SDL_Rect gameOverHomeButtonRect = {515, 413, 229, 58};
const SDL_Rect backButtonRect = {0, 0, 70, 70};  // Nút back
const SDL_Rect decreaseButtonRect = {172, 293, 131, 100}; // Nút giảm âm lượng
const SDL_Rect increaseButtonRect = {709, 293, 131, 100}; // Nút tăng âm lượng

void playSoundEffect(Mix_Chunk* sound, int loops) {
    if (sound) Mix_PlayChannel(-1, sound, loops);
}

void stopCurrentSound() {
    if (currentSoundChannel != -1) {
        Mix_HaltChannel(currentSoundChannel);
        currentSoundChannel = -1;
    }
}

void playLoopingSound(Mix_Chunk* sound) {
    stopCurrentSound();
    if (sound) currentSoundChannel = Mix_PlayChannel(-1, sound, -1);
}

void loadSounds() {
    menuSound = Mix_LoadWAV("sounds/menu_sound.wav");
    gameSound = Mix_LoadWAV("sounds/play_sound.wav");
    jumpSound = Mix_LoadWAV("sounds/jump_sound.wav");
    loseSound = Mix_LoadWAV("sounds/lose_sound.wav");
    clickSound = Mix_LoadWAV("sounds/click_sound.wav");
    adjustVolume(0); // Khởi tạo âm lượng mặc định
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

bool loadFont() {
    font = TTF_OpenFont("VCOOPERB.ttf", 30);
    return font != nullptr;
}

void freeFont() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

SDL_Texture* renderText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!textSurface) return nullptr;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}

bool loadTextures() {
    mainScreenTexture = LoadTexture("imgs/Background/MainScreen.png", renderer);
    instructionTexture = LoadTexture("imgs/Background/Introduction.png", renderer);
    pauseButtonTexture = LoadTexture("imgs/Buttons/Pause.png", renderer);
    pauseScreenTexture = LoadTexture("imgs/Background/PauseScreen.png", renderer);
    gameOverTexture = LoadTexture("imgs/Buttons/GameOver.png", renderer);
    endScreenTexture = LoadTexture("imgs/Background/EndScreen.png", renderer);
    settingsTexture = LoadTexture("imgs/Background/Setting.png", renderer); // Tải texture Settings

    return mainScreenTexture && instructionTexture && pauseButtonTexture &&
           pauseScreenTexture && gameOverTexture && endScreenTexture && settingsTexture;
}

void freeTextures() {
    if (mainScreenTexture) SDL_DestroyTexture(mainScreenTexture);
    if (instructionTexture) SDL_DestroyTexture(instructionTexture);
    if (pauseButtonTexture) SDL_DestroyTexture(pauseButtonTexture);
    if (pauseScreenTexture) SDL_DestroyTexture(pauseScreenTexture);
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (endScreenTexture) SDL_DestroyTexture(endScreenTexture);
    if (settingsTexture) SDL_DestroyTexture(settingsTexture); // Giải phóng texture Settings

    mainScreenTexture = nullptr;
    instructionTexture = nullptr;
    pauseButtonTexture = nullptr;
    pauseScreenTexture = nullptr;
    gameOverTexture = nullptr;
    endScreenTexture = nullptr;
    settingsTexture = nullptr;
}

void renderHelpScreen(SDL_Renderer* renderer) {
    if (instructionTexture) {
        SDL_RenderCopy(renderer, instructionTexture, nullptr, nullptr);
    }
}

void renderMainScreen(SDL_Renderer* renderer) {
    if (mainScreenTexture) {
        SDL_RenderCopy(renderer, mainScreenTexture, nullptr, nullptr);
    }
}

void renderPauseScreen(SDL_Renderer* renderer) {
    if (pauseScreenTexture) {
        const int PAUSE_WIDTH = 400;
        const int PAUSE_HEIGHT = 240;
        SDL_Rect destRect = {(SCREEN_WIDTH - PAUSE_WIDTH) / 2, (SCREEN_HEIGHT - PAUSE_HEIGHT) / 2, PAUSE_WIDTH, PAUSE_HEIGHT};
        SDL_RenderCopy(renderer, pauseScreenTexture, nullptr, &destRect);
    }
}

void renderSettingsScreen(SDL_Renderer* renderer) {
    if (settingsTexture) {
        SDL_Rect settingsRect = {(SCREEN_WIDTH - 1000) / 2, (SCREEN_HEIGHT - 600) / 2, 1000, 600};
        SDL_RenderCopy(renderer, settingsTexture, nullptr, &settingsRect);

        // Vẽ thanh âm lượng
        SDL_Rect volumeBar = {300, 318, 400, 50}; // Thanh trắng nền
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &volumeBar);

        // Vẽ phần màu đen biểu thị âm lượng (tỷ lệ với 400px)
        SDL_Rect filledVolume = {300, 318, (volume * 400) / 100, 50};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &filledVolume);
    }
}

void adjustVolume(int change) {
    volume += change;
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    Mix_Volume(-1, (volume * MIX_MAX_VOLUME) / 100); // Áp dụng âm lượng cho tất cả các kênh
    Mix_VolumeChunk(menuSound, (volume * MIX_MAX_VOLUME) / 100);
    Mix_VolumeChunk(gameSound, (volume * MIX_MAX_VOLUME) / 100);
    Mix_VolumeChunk(jumpSound, (volume * MIX_MAX_VOLUME) / 100);
    Mix_VolumeChunk(loseSound, (volume * MIX_MAX_VOLUME) / 100);
    Mix_VolumeChunk(clickSound, (volume * MIX_MAX_VOLUME) / 100);
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
    // Vẽ overlay đen mờ
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    // Vẽ ảnh GameOver
    if (gameOverTexture) {
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    }

    // Vẽ text score
    std::string scoreText = "Your Score: " + std::to_string(score);
    SDL_Texture* scoreTextTexture = renderText(scoreText, renderer, font, goldColor);
    if (scoreTextTexture) {
        SDL_Rect scoreRect;
        SDL_QueryTexture(scoreTextTexture, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
        scoreRect.x = SCREEN_WIDTH/2 - scoreRect.w/2;
        scoreRect.y = SCREEN_HEIGHT/2 - 50;
        SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTextTexture);
    }

    // Vẽ text high score
    std::string highScoreText = "Highest Score: " + std::to_string(highScore);
    SDL_Texture* highScoreTexture = renderText(highScoreText, renderer, font, goldColor);
    if (highScoreTexture) {
        SDL_Rect highScoreRect;
        SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreRect.w, &highScoreRect.h);
        highScoreRect.x = SCREEN_WIDTH/2 - highScoreRect.w/2;
        highScoreRect.y = SCREEN_HEIGHT/2;
        SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
        SDL_DestroyTexture(highScoreTexture);
    }

    // Vẽ end screen với các nút
    if (endScreenTexture) {
        SDL_RenderCopy(renderer, endScreenTexture, nullptr, &endScreenRect);
    }
}

