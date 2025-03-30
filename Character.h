#ifndef CHARACTER_H
#define CHARACTER_H

#include "CommonFunc.h"
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"

using namespace std;

struct Dino {
    int x, y;
    bool isJumping;
    bool isCrouching;
    int jumpVelocity;
    int frameIndex;
    Uint32 crouchTime;
};

void initCharacter();
void handleInput(SDL_Event& e, Dino& dino);
void updateCharacter(Dino& dino, Uint32& dinoFrameTime);
void renderCharacter(Dino& dino);
void closeCharacter();

#endif
