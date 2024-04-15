#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <SDL2/SDL.h>

struct Point
{
    float x, y;
    float xVelocity, yVelocity;
    float mass;
    SDL_Color color;
};

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int POINT_SIZE = 1;
int NUM_POINTS = 500;
float MAX_VELOCITY = 10.0f;
float BASE_GRAVITY_CONSTANT = 0.0008f;
float MAX_GRAVITY_RANGE = 150.0f;
float MOUSE_INFLUENCE_RADIUS = 50.0f;
float FRICTION_FACTOR = 0.2f;
float GRAVITY = 0.1f;

#endif