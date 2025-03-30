#include <iostream>
#include <vector>
#include <fstream>
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "Utils.h"

// Game states
bool isGameOver = false;
bool isPaused = false;
bool showMainScreen = true;

// Game variables
int score = 0;
int highScore = 0;
Uint32 scoreTimer = 0;
const int SCORE_INTERVAL = 100;

void loadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void saveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void resetGame(Dino& dino, vector<CACTUS>& cacti, vector<BIRD>& birds, int& obstacleSpeed, int& timeCounter) {
    dino = {100, GROUND_HEIGHT - CHARACTER_HEIGHT, false, false, 0, 0, 0};
    cacti.clear();
    birds.clear();
    initializeObstacles(cacti, birds, 5);
    obstacleSpeed = 15;
    timeCounter = 0;
    score = 0;
    scoreTimer = SDL_GetTicks();
    isGameOver = false;
    isPaused = false;
    showMainScreen = false;
    playLoopingSound(gameSound);
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

    if (!loadFont()) return -1;
    loadSounds();
    loadHighScore();

    initBackgrounds();
    initCharacter();
    initCactus();
    initBird();

    if (!loadTextures()) return -1;

    Dino dino = {100, GROUND_HEIGHT - CHARACTER_HEIGHT, false, false, 0, 0, 0};
    vector<CACTUS> cacti;
    vector<BIRD> birds;
    initializeObstacles(cacti, birds, 5);

    bool isRunning = true;
    int timeCounter = 0;
    int obstacleSpeed = 15;
    SDL_Event e;

    Uint32 lastTime = SDL_GetTicks();
    Uint32 dinoFrameTime = SDL_GetTicks();
    scoreTimer = SDL_GetTicks();

    playLoopingSound(menuSound);

    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                isRunning = false;
            }

            if (showMainScreen) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (x >= playButtonRect.x && x <= playButtonRect.x + playButtonRect.w &&
                        y >= playButtonRect.y && y <= playButtonRect.y + playButtonRect.h) {
                        playSoundEffect(clickSound);
                        resetGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                    }
                }
                else if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN)) {
                    playSoundEffect(clickSound);
                    resetGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                }
            }
            else {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        playSoundEffect(clickSound);
                        isPaused = !isPaused;
                    }
                    else if (isPaused && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN)) {
                        playSoundEffect(clickSound);
                        isPaused = false;
                    }
                    else if (!isPaused && !isGameOver) {
                        if ((e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) && !dino.isJumping) {
                            playSoundEffect(jumpSound);
                        }
                        handleInput(e, dino);
                    }
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    playSoundEffect(clickSound);
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (isGameOver) {
                        if (x >= replayButtonRect.x && x <= replayButtonRect.x + replayButtonRect.w &&
                            y >= replayButtonRect.y && y <= replayButtonRect.y + replayButtonRect.h) {
                            resetGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                        }
                        else if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
                                 y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
                            isRunning = false;
                        }
                        else if (x >= homeButtonRect.x && x <= homeButtonRect.x + homeButtonRect.w &&
                                 y >= homeButtonRect.y && y <= homeButtonRect.y + homeButtonRect.h) {
                            showMainScreen = true;
                            isGameOver = false;
                            playLoopingSound(menuSound);
                        }
                    }
                    else if (isPaused) {
                        if (x >= continueButtonRect.x && x <= continueButtonRect.x + continueButtonRect.w &&
                            y >= continueButtonRect.y && y <= continueButtonRect.y + continueButtonRect.h) {
                            isPaused = false;
                        }
                        else if (x >= pauseHomeButtonRect.x && x <= pauseHomeButtonRect.x + pauseHomeButtonRect.w &&
                                 y >= pauseHomeButtonRect.y && y <= pauseHomeButtonRect.y + pauseHomeButtonRect.h) {
                            showMainScreen = true;
                            isPaused = false;
                            playLoopingSound(menuSound);
                        }
                    }
                    else if (!isPaused && x >= pauseButtonRect.x && x <= pauseButtonRect.x + pauseButtonRect.w &&
                             y >= pauseButtonRect.y && y <= pauseButtonRect.y + pauseButtonRect.h) {
                        isPaused = true;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (showMainScreen) {
            renderMainScreen(renderer);
        }
        else {
            renderBackground();
            renderCharacter(dino);
            renderObstacles(cacti, birds);
            renderInGameScores(renderer, score, highScore);

            if (!isPaused && !isGameOver) {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - scoreTimer > SCORE_INTERVAL) {
                    score++;
                    scoreTimer = currentTime;

                    if (score > highScore) {
                        highScore = score;
                    }
                }

                if (pauseButtonTexture) {
                    SDL_RenderCopy(renderer, pauseButtonTexture, nullptr, &pauseButtonRect);
                }

                updateBackground();
                updateCharacter(dino, dinoFrameTime);
                updateObstacles(cacti, birds, obstacleSpeed);
                adjustSpeed(timeCounter, lastTime, obstacleSpeed);

                if (checkCollision(dino, cacti, birds)) {
                    isGameOver = true;
                    playSoundEffect(loseSound);
                    saveHighScore();
                    stopCurrentSound();
                }
            }
            else if (isPaused) {
                renderPauseScreen(renderer);
            }

            if (isGameOver) {
                renderGameOverScreen(renderer, score, highScore);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(FRAME_DELAY);
    }

    freeTextures();
    freeSounds();
    freeFont();
    TTF_Quit();
    Mix_CloseAudio();

    closeObstacles();
    closeBackgrounds();
    closeCharacter();
    close();

    return 0;
}
