#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"

const int LAYER_COUNT = 5;
SDL_Texture* Layers[LAYER_COUNT];
double bgX1[LAYER_COUNT], bgX2[LAYER_COUNT];

const double Layer_speed[LAYER_COUNT] = {0.5, 0.9, 1.3, 1.6, 0}; // Giá trị cho Land_1 không quan trọng vì sẽ bị ghi đè
const double Layer_cut[LAYER_COUNT] = {0, 0, 0, 0, 0};
const char* Layer_path[LAYER_COUNT] = {
    "imgs/Background/Hill_1.png",
    "imgs/Background/Bush_1.png",
    "imgs/Background/Bush_2.png",
    "imgs/Background/Bush_3.png",
    "imgs/Background/Land_1.png",
};

void initBackgrounds() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        Layers[i] = LoadTexture(Layer_path[i], renderer);
        if (!Layers[i]) {
            cerr << "Failed to load background layer: " << Layer_path[i] << endl;
        }
        bgX1[i] = 0;
        bgX2[i] = SCREEN_WIDTH;
    }
}

void updateBackground(int obstacleSpeed) {
    for (int i = 0; i < LAYER_COUNT; i++) {
        double speed = (i == 4) ? obstacleSpeed : Layer_speed[i]; // Layer 4 là Land_1
        bgX1[i] -= speed;
        bgX2[i] -= speed;

        if (bgX1[i] <= -SCREEN_WIDTH) bgX1[i] = bgX2[i] + SCREEN_WIDTH;
        if (bgX2[i] <= -SCREEN_WIDTH) bgX2[i] = bgX1[i] + SCREEN_WIDTH;
    }
}

void renderBackground() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        SDL_Rect bgRect1_src = {0, (int)Layer_cut[i], SCREEN_WIDTH, SCREEN_HEIGHT - (int)Layer_cut[i]};
        SDL_Rect bgRect2_src = {0, (int)Layer_cut[i], SCREEN_WIDTH, SCREEN_HEIGHT - (int)Layer_cut[i]};
        SDL_Rect bgRect1_dest = {(int)bgX1[i], 0, SCREEN_WIDTH, SCREEN_HEIGHT - (int)Layer_cut[i]};
        SDL_Rect bgRect2_dest = {(int)bgX2[i], 0, SCREEN_WIDTH, SCREEN_HEIGHT - (int)Layer_cut[i]};

        SDL_RenderCopy(renderer, Layers[i], &bgRect1_src, &bgRect1_dest);
        SDL_RenderCopy(renderer, Layers[i], &bgRect2_src, &bgRect2_dest);
    }
}

void closeBackgrounds() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        SDL_DestroyTexture(Layers[i]);
        Layers[i] = nullptr;
    }
}
