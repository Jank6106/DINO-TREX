#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "CommonFunc.h"
#include "LTexture.h"
#include "CommonFunc.h"
#include "Background.h"
#include "Character.h"
#include <vector>

using namespace std;

struct CACTUS {
    int x, y;
};

struct BIRD {
    int x, y;
    int frameIndex;
    Uint32 frameTime;
    bool isHighPosition; // true for high position, false for low position
};

void initCactus();
void initBird();
void initializeObstacles(vector<CACTUS>& cacti, vector<BIRD>& birds, int count);
void updateObstacles(vector<CACTUS>& cacti, vector<BIRD>& birds, int obstacleSpeed);
bool checkCollision(const Dino& dino, const vector<CACTUS>& cacti, const vector<BIRD>& birds);
void renderObstacles(const vector<CACTUS>& cacti, const vector<BIRD>& birds);
void closeObstacles();

#endif
