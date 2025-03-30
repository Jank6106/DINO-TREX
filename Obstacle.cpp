#include "Obstacle.h"
#include "CommonFunc.h"
#include "LTexture.h"
#include "Character.h"
#include "Background.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

SDL_Texture* Cactus;
SDL_Texture* Bird;
const int COLLISION_MARGIN = 25;
const int COLLISION_MARGIN_2 = 20;
const int MIN_DISTANCE = 200;
const int MAX_DISTANCE = 1500;
const int BIRD_WIDTH = 183;
const int BIRD_HEIGHT = 168;
const int BIRD_SCALED_WIDTH = 80;
const int BIRD_SCALED_HEIGHT = 74;
const int BIRD_ANIMATION_DELAY = 100; // ms between frames
const int HIGH_BIRD_Y = GROUND_HEIGHT - 200; // High position
const int LOW_BIRD_Y = GROUND_HEIGHT - 130; // Low position

// Bird animation frames (x, y, w, h)
SDL_Rect birdFrames[8] = {
    {732, 0, 183, 168},
    {366, 0, 183, 168},
    {0, 0, 183, 168},
    {732, 168, 183, 168},
    {366, 168, 183, 168},
    {549, 336, 183, 168},
    {366, 336, 183, 168},
    {183, 336, 183, 168}
};

const char* CACTUS_PATH = "imgs/Obstacle/Cactus.png";
const char* BIRD_PATH = "imgs/Obstacle/Bird.png";

void initCactus() {
    Cactus = LoadTexture(CACTUS_PATH, renderer);
    if (!Cactus) {
        cerr << "Failed to load cactus texture!" << endl;
    }
}

void initBird() {
    Bird = LoadTexture(BIRD_PATH, renderer);
    if (!Bird) {
        cerr << "Failed to load bird texture!" << endl;
    }
}

void initializeObstacles(vector<CACTUS>& cacti, vector<BIRD>& birds, int count) {
    srand(static_cast<unsigned int>(time(0)));
    int lastX = SCREEN_WIDTH;

    for (int i = 0; i < count; ++i) {
        // Randomly decide if this will be a cactus or bird
        bool createBird = (rand() % 3) == 0; // 1/3 chance to create bird

        int randomDistance = MIN_DISTANCE + rand() % (MAX_DISTANCE - MIN_DISTANCE);
        lastX += randomDistance;

        if (createBird) {
            bool isHighPosition = (rand() % 2) == 0; // 50% chance for high position
            birds.push_back({
                lastX,
                isHighPosition ? HIGH_BIRD_Y : LOW_BIRD_Y,
                0, // frameIndex
                SDL_GetTicks(), // frameTime
                isHighPosition
            });
        } else {
            cacti.push_back({lastX, GROUND_HEIGHT - CACTUS_HEIGHT});
        }
    }
}

void updateObstacles(vector<CACTUS>& cacti, vector<BIRD>& birds, int obstacleSpeed) {
    // Update cacti
    for (auto& cactus : cacti) {
        cactus.x -= obstacleSpeed;
        if (cactus.x + CACTUS_WIDTH < 0) {
            int farthestX = 0;
            for (const auto& other : cacti) {
                if (other.x > farthestX) farthestX = other.x;
            }
            for (const auto& bird : birds) {
                if (bird.x > farthestX) farthestX = bird.x;
            }
            int randomDistance = MIN_DISTANCE + rand() % (MAX_DISTANCE - MIN_DISTANCE);
            cactus.x = farthestX + randomDistance;
            cactus.y = GROUND_HEIGHT - CACTUS_HEIGHT;
        }
    }

    // Update birds
    for (auto& bird : birds) {
        bird.x -= obstacleSpeed;

        // Update animation
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - bird.frameTime >= BIRD_ANIMATION_DELAY) {
            bird.frameIndex = (bird.frameIndex + 1) % 8;
            bird.frameTime = currentTime;
        }

        if (bird.x + BIRD_SCALED_WIDTH < 0) {
            int farthestX = 0;
            for (const auto& other : cacti) {
                if (other.x > farthestX) farthestX = other.x;
            }
            for (const auto& otherBird : birds) {
                if (otherBird.x > farthestX) farthestX = otherBird.x;
            }
            int randomDistance = MIN_DISTANCE + rand() % (MAX_DISTANCE - MIN_DISTANCE);
            bird.x = farthestX + randomDistance;
            bird.isHighPosition = (rand() % 2) == 0;
            bird.y = bird.isHighPosition ? HIGH_BIRD_Y : LOW_BIRD_Y;
        }
    }
}

bool checkCollision(const Dino& dino, const vector<CACTUS>& cacti, const vector<BIRD>& birds) {
    SDL_Rect dinoRect = {dino.x + COLLISION_MARGIN, dino.y + COLLISION_MARGIN,
                         CHARACTER_WIDTH - 2 * COLLISION_MARGIN, CHARACTER_HEIGHT - 2 * COLLISION_MARGIN};

    // Check cactus collisions
    for (const auto& cactus : cacti) {
        SDL_Rect cactusRect = {cactus.x + COLLISION_MARGIN, cactus.y + COLLISION_MARGIN,
                               CACTUS_WIDTH - 1 * COLLISION_MARGIN, CACTUS_HEIGHT - 1 * COLLISION_MARGIN};
        if (SDL_HasIntersection(&dinoRect, &cactusRect)) {
            return true;
        }
    }

    // Check bird collisions
    for (const auto& bird : birds) {
        SDL_Rect birdRect = {bird.x + COLLISION_MARGIN_2, bird.y + COLLISION_MARGIN_2, BIRD_SCALED_WIDTH - 1 * COLLISION_MARGIN_2, BIRD_SCALED_HEIGHT - 1 * COLLISION_MARGIN_2};

        // For high birds, only check collision if dino is jumping
        if (bird.isHighPosition) {
            if (dino.isJumping && SDL_HasIntersection(&dinoRect, &birdRect)) {
                return true;
            }
        }
        // For low birds, check collision unless dino is crouching
        else {
            if (!dino.isCrouching && SDL_HasIntersection(&dinoRect, &birdRect)) {
                return true;
            }
        }
    }

    return false;
}

void renderObstacles(const vector<CACTUS>& cacti, const vector<BIRD>& birds) {
    // Render cacti
    for (const auto& cactus : cacti) {
        SDL_Rect cactusRect = {cactus.x, cactus.y, CACTUS_WIDTH, CACTUS_HEIGHT};
        SDL_RenderCopy(renderer, Cactus, nullptr, &cactusRect);
    }

    // Render birds
    for (const auto& bird : birds) {
        SDL_Rect destRect = {bird.x, bird.y, BIRD_SCALED_WIDTH, BIRD_SCALED_HEIGHT};
        SDL_Rect srcRect = birdFrames[bird.frameIndex];
        SDL_RenderCopy(renderer, Bird, &srcRect, &destRect);
    }
}

void closeObstacles() {
    if (Cactus != nullptr) {
        SDL_DestroyTexture(Cactus);
        Cactus = nullptr;
    }
    if (Bird != nullptr) {
        SDL_DestroyTexture(Bird);
        Bird = nullptr;
    }
}
