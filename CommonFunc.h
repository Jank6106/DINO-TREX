#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern bool isPaused;

const char WINDOW_TITLE[] = "DINO T-REX GAME";

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int GROUND_HEIGHT = 570;
const int CHARACTER_WIDTH = 100;
const int CHARACTER_HEIGHT = 90;
const int CACTUS_WIDTH = 60;
const int CACTUS_HEIGHT = 60;
static const int Ground = 300;
const int FRAME_DELAY = 30;
const int SPEED_INCREMENT_INTERVAL = 5000;
const int SPEED_INCREMENT = 1;


bool init();
void adjustSpeed(int& timeCounter, Uint32& lastTime, int& obstacleSpeed);
void close();

#endif // COMMONFUNC_H_INCLUDED
