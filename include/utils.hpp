#ifndef UTILS_HPP
#define UTILS_HPP

#include "params.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

int generate_random_int(int min, int max);

float generate_random_float(float min, float max);

float calculate_distance(float x1, float y1, float x2, float y2);

float calculate_gravity_force(const Point &point, const Point &otherPoint);

void apply_gravity(Point &point, const Point &otherPoint);

void limit_velocity(Point &point);

bool check_collision(const Point &point, const Point &point2);

void handle_collision(Point &point, Point &point2);

void read_params();

#endif