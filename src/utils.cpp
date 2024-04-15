#include "../include/utils.hpp"

int generate_random_int(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

float generate_random_float(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

float calculate_distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float calculate_gravity_force(const Point &point, const Point &otherPoint)
{
    float distance = calculate_distance(point.x, point.y, otherPoint.x, otherPoint.y);
    float force = BASE_GRAVITY_CONSTANT * point.mass * otherPoint.mass / pow(distance, 2);
    return force;
}

void apply_gravity(Point &point, const Point &otherPoint)
{
    float force = calculate_gravity_force(point, otherPoint);
    float angle = atan2(otherPoint.y - point.y, otherPoint.x - point.x);
    point.xVelocity += cos(angle) * force / point.mass;
    point.yVelocity += sin(angle) * force / point.mass;
}

void limit_velocity(Point &point)
{
    float velocity = sqrt(pow(point.xVelocity, 2) + pow(point.yVelocity, 2));
    if (velocity > MAX_VELOCITY)
    {
        float angle = atan2(point.yVelocity, point.xVelocity);
        point.xVelocity = cos(angle) * MAX_VELOCITY;
        point.yVelocity = sin(angle) * MAX_VELOCITY;
    }
}

bool check_collision(const Point &point, const Point &point2)
{
    return calculate_distance(point.x, point.y, point2.x, point2.y) < POINT_SIZE;
}

void handle_collision(Point &point, Point &point2)
{
    float angle = atan2(point2.y - point.y, point2.x - point.x);
    point.xVelocity = cos(angle) * MAX_VELOCITY;
    point.yVelocity = sin(angle) * MAX_VELOCITY;
}

void read_params()
{
    std::ifstream file("params.txt");
    if (file.is_open())
    {
        file >> GRAVITY;
        file >> BASE_GRAVITY_CONSTANT;
        file >> MAX_VELOCITY;
        file >> POINT_SIZE;
        file >> SCREEN_WIDTH;
        file >> SCREEN_HEIGHT;
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open params.txt" << std::endl;
    }
}