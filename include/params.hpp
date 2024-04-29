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

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int POINT_SIZE;
extern int NUM_POINTS;
extern float MAX_VELOCITY;
extern float BASE_GRAVITY_CONSTANT;
extern float MAX_GRAVITY_RANGE;
extern float MOUSE_INFLUENCE_RADIUS;
extern float FRICTION_FACTOR;
extern float GRAVITY;

#endif