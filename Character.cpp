#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "Utils.h"

const int MAX_JUMP_VELOCITY = 40;
const int GRAVITY = 5;
const int CHALAYERS_COUNT = 8;
const Uint32 CROUCH_DURATION = 500;

SDL_Texture* ChaLayers[CHALAYERS_COUNT];
SDL_Texture* jumpLayers;
SDL_Texture* crouchLayers;

const char* Character_path[CHALAYERS_COUNT] = {
    "imgs/Character/Run (1).png",
    "imgs/Character/Run (2).png",
    "imgs/Character/Run (3).png",
    "imgs/Character/Run (4).png",
    "imgs/Character/Run (5).png",
    "imgs/Character/Run (6).png",
    "imgs/Character/Run (7).png",
    "imgs/Character/Run (8).png"
};
const char* Jump_path = "imgs/Character/Jump (8).png";
const char* crouch_path = "imgs/Character/Crouch (8).png";

void initCharacter() {
    for (int i = 0; i < CHALAYERS_COUNT; i++) {
        ChaLayers[i] = LoadTexture(Character_path[i], renderer);
    }
    jumpLayers = LoadTexture(Jump_path, renderer);
    crouchLayers = LoadTexture(crouch_path, renderer);
}

void handleInput(SDL_Event& e, Dino& dino) {
    if (isPaused || isGameOver) return;

    if (e.type == SDL_KEYDOWN) {
        if ((e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) && !dino.isJumping) {
            dino.isJumping = true;
            dino.jumpVelocity = MAX_JUMP_VELOCITY;
            if (jumpSound) {
                playSoundEffect(jumpSound);
            }
        }
        else if (e.key.keysym.sym == SDLK_DOWN && !dino.isJumping) {
            dino.isCrouching = true;
            dino.crouchStartTime = SDL_GetTicks();
        }
    }
}

void updateCharacter(Dino& dino, Uint32& dinoFrameTime) {
    // Xử lý nhảy
    if (dino.isJumping) {
        dino.y -= dino.jumpVelocity;
        dino.jumpVelocity -= GRAVITY;

        if (dino.y >= GROUND_HEIGHT - CHARACTER_HEIGHT) {
            dino.y = GROUND_HEIGHT - CHARACTER_HEIGHT;
            dino.isJumping = false;
        }
    }

    // Xử lý cúi
    if (dino.isCrouching) {
        Uint32 currentTime = SDL_GetTicks();

        // Tự động đứng dậy sau CROUCH_DURATION dù vẫn đang giữ phím
        if (currentTime - dino.crouchStartTime >= CROUCH_DURATION) {
            dino.isCrouching = false;
        }
    }

    // Cập nhật animation chạy khi không nhảy và không cúi
    if (!dino.isJumping && !dino.isCrouching) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - dinoFrameTime >= 50) {
            dino.frameIndex = (dino.frameIndex + 1) % CHALAYERS_COUNT;
            dinoFrameTime = currentTime;
        }
    }
}

void renderCharacter(Dino& dino) {
    SDL_Rect destRect = {dino.x, dino.y, CHARACTER_WIDTH, CHARACTER_HEIGHT};

    if (dino.isJumping) {
        SDL_RenderCopy(renderer, jumpLayers, nullptr, &destRect);
    }
    else if (dino.isCrouching) {
        SDL_RenderCopy(renderer, crouchLayers, nullptr, &destRect);
    }
    else {
        SDL_RenderCopy(renderer, ChaLayers[dino.frameIndex], nullptr, &destRect);
    }
}

void closeCharacter() {
    for (int i = 0; i < CHALAYERS_COUNT; i++) {
        if (ChaLayers[i]) SDL_DestroyTexture(ChaLayers[i]);
    }
    if (jumpLayers) SDL_DestroyTexture(jumpLayers);
    if (crouchLayers) SDL_DestroyTexture(crouchLayers);
}
