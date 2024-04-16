#ifndef UTILS_HPP
#define UTILS_HPP

#include "params.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

int generate_random_int(int min, int max);

float generate_random_float(float min, float max);

float calculate_distance(float x1, float y1, float x2, float y2);

float calculate_gravity_force(const Point &point, const Point &otherPoint);

void apply_gravity(Point &point, const Point &otherPoint);

void limit_velocity(Point &point);

bool check_collision(const Point &point, const Point &point2);

void handle_collision(Point &point, Point &point2);

void read_params(int &SCREEN_WIDTH, int &SCREEN_HEIGHT, int &POINT_SIZE, int &NUM_POINTS, float &MAX_VELOCITY,
                 float &BASE_GRAVITY_CONSTANT, float &MAX_GRAVITY_RANGE, float &MOUSE_INFLUENCE_RADIUS,
                 float &FRICTION_FACTOR, float &GRAVITY);

#endif