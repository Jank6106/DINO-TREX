#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include <SDL.h>
#include <string>
#include "LTexture.h"
#include <iostream>

using namespace std;

void initBackgrounds();
void updateBackground();
void renderBackground();
void closeBackgrounds();

#endif
