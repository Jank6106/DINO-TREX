#include "Character.h"
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Obstacle.h"

const int MAX_JUMP_VELOCITY = 40;
const int GRAVITY = 5;
const int CHALAYERS_COUNT = 8;
const Uint32 CROUCH_DURATION = 500; // Thời gian cúi (0.5 giây)

SDL_Texture* ChaLayers[CHALAYERS_COUNT];
SDL_Texture* jumpLayers;
SDL_Texture* crouchLayers;

int frameDelay = 100;
int frame_count = 0;

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
        if (!ChaLayers[i]) {
            cerr << "Failed to load character layer: " << Character_path[i] << endl;
        }
    }

    jumpLayers = LoadTexture(Jump_path, renderer);
    if (!jumpLayers) {
        cerr << "Failed to load jump layer!" << endl;
    }

    crouchLayers = LoadTexture(crouch_path, renderer);
    if (!crouchLayers) {
        cerr << "Failed to load crouch layer!" << endl;
    }
}

void handleInput(SDL_Event& e, Dino& dino) {
    if (isPaused) return;

    if (e.type == SDL_KEYDOWN) {
        if ((e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) && !dino.isJumping) {
            dino.isJumping = true;
            dino.jumpVelocity = MAX_JUMP_VELOCITY;
        }
        else if (e.key.keysym.sym == SDLK_DOWN && !dino.isJumping) {
            dino.isCrouching = true;
            dino.crouchTime = SDL_GetTicks(); // Lưu thời điểm bắt đầu cúi
        }
    }
    // Bỏ xử lý SDL_KEYUP cho phím xuống
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

    // Tự động đứng dậy sau khi hết thời gian cúi
    if (dino.isCrouching && (SDL_GetTicks() - dino.crouchTime > CROUCH_DURATION)) {
        dino.isCrouching = false;
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
    SDL_Rect ChaRect_des = {dino.x, dino.y, CHARACTER_WIDTH, CHARACTER_HEIGHT};
    if (dino.isJumping) {
        SDL_RenderCopy(renderer, jumpLayers, nullptr, &ChaRect_des);
    }
    else if (dino.isCrouching) {
        SDL_RenderCopy(renderer, crouchLayers, nullptr, &ChaRect_des);
    }
    else {
        SDL_RenderCopy(renderer, ChaLayers[dino.frameIndex], nullptr, &ChaRect_des);
    }
}

void closeCharacter() {
    for (int i = 0; i < CHALAYERS_COUNT; i++) {
        if (ChaLayers[i] != nullptr) {
            SDL_DestroyTexture(ChaLayers[i]);
            ChaLayers[i] = nullptr;
        }
    }

    if (jumpLayers) {
        SDL_DestroyTexture(jumpLayers);
        jumpLayers = nullptr;
    }

    if (crouchLayers) {
        SDL_DestroyTexture(crouchLayers);
        crouchLayers = nullptr;
    }
}
