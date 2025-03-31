#include <iostream>
#include <vector>
#include <fstream>
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "Utils.h"

bool isGameOver = false;
bool isPaused = false;
bool showMainScreen = true;
bool showHelpScreen = false;
bool showSettingsScreen = false; // Thêm biến để hiển thị màn hình Settings

int score = 0;
int highScore = 0;
Uint32 scoreTimer = 0;
const int SCORE_INTERVAL = 100;
const int INITIAL_OBSTACLE_SPEED = 15;

// đọc điểm cao nhất từ file
void loadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) file >> highScore;
}

// lưu điểm cao nhất vào file
void saveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) file << highScore;
}

// khởi động trò chơi mới
void startNewGame(Dino& dino, std::vector<CACTUS>& cacti, std::vector<BIRD>& birds, int& obstacleSpeed, int& timeCounter) {
    dino = {100, GROUND_HEIGHT - CHARACTER_HEIGHT, false, false, 0, 0, 0}; // thiết lập dino
    cacti.clear();
    birds.clear();
    initializeObstacles(cacti, birds, 5); // tạo chướng ngại vật mới
    obstacleSpeed = INITIAL_OBSTACLE_SPEED;
    timeCounter = 0;
    score = 0;
    scoreTimer = SDL_GetTicks();
    isGameOver = false;
    isPaused = false;
    showMainScreen = false;
    showHelpScreen = false;
    showSettingsScreen = false; // Đảm bảo thoát khỏi Settings khi bắt đầu game mới
    stopCurrentSound();
    playLoopingSound(gameSound);
}

int main(int argc, char* args[]) {
    if (!init()) return -1; // load SDL
    if (!loadFont()) return -1; // load font

    loadSounds(); // Tải âm thanh
    loadHighScore(); // Tải điểm cao nhất
    initBackgrounds(); // Khởi tạo nền
    initCharacter(); // Khởi tạo Dino
    initCactus(); // Khởi tạo cactus
    initBird(); // Khởi tạo bird
    if (!loadTextures()) return -1; // Tải texture giao diện

    Dino dino;
    std::vector<CACTUS> cacti;
    std::vector<BIRD> birds;
    bool isRunning = true;
    int timeCounter = 0;
    int obstacleSpeed = INITIAL_OBSTACLE_SPEED;
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 dinoFrameTime = SDL_GetTicks();
    scoreTimer = SDL_GetTicks();

    playLoopingSound(menuSound);

    // vòng lặp game
    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) isRunning = false; // thoát game khi đóng cửa sổ

            if (showMainScreen && !showHelpScreen && !showSettingsScreen) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
                        y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
                        playSoundEffect(clickSound);
                        startNewGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                    }
                    else if (x >= helpButtonRect.x && x <= helpButtonRect.x + helpButtonRect.w &&
                             y >= helpButtonRect.y && y <= helpButtonRect.y + helpButtonRect.h) {
                        playSoundEffect(clickSound);
                        showHelpScreen = true;
                    }
                    else if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
                             y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
                        playSoundEffect(clickSound);
                        isRunning = false;
                    }
                    else if (x >= OptionsRect.x && x <= OptionsRect.x + OptionsRect.w &&
                             y >= OptionsRect.y && y <= OptionsRect.y + OptionsRect.h) {
                        playSoundEffect(clickSound);
                        showSettingsScreen = true;
                    }
                }
                else if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN)) {
                    playSoundEffect(clickSound);
                    startNewGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                }
            }
            else if (showHelpScreen) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                        y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                        playSoundEffect(clickSound);
                        showHelpScreen = false;
                    }
                }
                else if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_BACKSPACE)) {
                    playSoundEffect(clickSound);
                    showHelpScreen = false;
                }
            }
            else if (showSettingsScreen) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (x >= decreaseButtonRect.x && x <= decreaseButtonRect.x + decreaseButtonRect.w &&
                        y >= decreaseButtonRect.y && y <= decreaseButtonRect.y + decreaseButtonRect.h) {
                        playSoundEffect(clickSound);
                        adjustVolume(-10); // Giảm âm lượng 10
                    }
                    else if (x >= increaseButtonRect.x && x <= increaseButtonRect.x + increaseButtonRect.w &&
                             y >= increaseButtonRect.y && y <= increaseButtonRect.y + increaseButtonRect.h) {
                        playSoundEffect(clickSound);
                        adjustVolume(10); // Tăng âm lượng 10
                    }
                    else if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                             y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                        playSoundEffect(clickSound);
                        showSettingsScreen = false;
                    }
                }
            }
            else {
                if (isPaused) {
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if (x >= continueButtonRect.x && x <= continueButtonRect.x + continueButtonRect.w &&
                            y >= continueButtonRect.y && y <= continueButtonRect.y + continueButtonRect.h) {
                            playSoundEffect(clickSound);
                            isPaused = false;
                        }
                        else if (x >= homeButtonRect.x && x <= homeButtonRect.x + homeButtonRect.w &&
                                 y >= homeButtonRect.y && y <= homeButtonRect.y + homeButtonRect.h) {
                            playSoundEffect(clickSound);
                            isPaused = false;
                            showMainScreen = true;
                            playLoopingSound(menuSound);
                        }
                    }
                    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                        playSoundEffect(clickSound);
                        isPaused = false;
                    }
                }
                else if (isGameOver) {
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if (x >= replayButtonRect.x && x <= replayButtonRect.x + replayButtonRect.w &&
                            y >= replayButtonRect.y && y <= replayButtonRect.y + replayButtonRect.h) {
                            playSoundEffect(clickSound);
                            startNewGame(dino, cacti, birds, obstacleSpeed, timeCounter);
                        }
                        else if (x >= gameOverHomeButtonRect.x && x <= gameOverHomeButtonRect.x + gameOverHomeButtonRect.w &&
                                 y >= gameOverHomeButtonRect.y && y <= gameOverHomeButtonRect.y + gameOverHomeButtonRect.h) {
                            playSoundEffect(clickSound);
                            showMainScreen = true;
                            isGameOver = false;
                            playLoopingSound(menuSound);
                        }
                    }
                }
                else {
                    if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_TAB) {
                            playSoundEffect(clickSound);
                            isPaused = true;
                        }
                        else if ((e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) && !dino.isJumping) {
                            playSoundEffect(jumpSound);
                            handleInput(e, dino);
                        }
                        else {
                            handleInput(e, dino);
                        }
                    }
                    else if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if (x >= pauseButtonRect.x && x <= pauseButtonRect.x + pauseButtonRect.w &&
                            y >= pauseButtonRect.y && y <= pauseButtonRect.y + pauseButtonRect.h) {
                            playSoundEffect(clickSound);
                            isPaused = true;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (showMainScreen) {
            if (showHelpScreen) {
                renderHelpScreen(renderer);
            }
            else if (showSettingsScreen) {
                renderSettingsScreen(renderer);
            }
            else {
                renderMainScreen(renderer);
            }
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
                    if (score > highScore) highScore = score;
                }

                if (pauseButtonTexture) {
                    SDL_RenderCopy(renderer, pauseButtonTexture, nullptr, &pauseButtonRect);
                }

                updateBackground(obstacleSpeed);
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
            else if (isGameOver) {
                renderGameOverScreen(renderer, score, highScore);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(FRAME_DELAY);
    }

    freeTextures();
    freeSounds();
    freeFont();
    closeObstacles();
    closeBackgrounds();
    closeCharacter();
    close();
    return 0;
}

