#include "../include/utils.hpp"

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

void read_params(int &SCREEN_WIDTH, int &SCREEN_HEIGHT, int &POINT_SIZE, int &NUM_POINTS, float &MAX_VELOCITY,
                 float &BASE_GRAVITY_CONSTANT, float &MAX_GRAVITY_RANGE, float &MOUSE_INFLUENCE_RADIUS,
                 float &FRICTION_FACTOR, float &GRAVITY)
{
    std::ifstream file("params.txt");
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (std::getline(is_line, value))
                {
                    std::stringstream val(value);
                    if (key == "SCREEN_WIDTH")
                        val >> SCREEN_WIDTH;
                    else if (key == "SCREEN_HEIGHT")
                        val >> SCREEN_HEIGHT;
                    else if (key == "POINT_SIZE")
                        val >> POINT_SIZE;
                    else if (key == "NUM_POINTS")
                        val >> NUM_POINTS;
                    else if (key == "MAX_VELOCITY")
                        val >> MAX_VELOCITY;
                    else if (key == "BASE_GRAVITY_CONSTANT")
                        val >> BASE_GRAVITY_CONSTANT;
                    else if (key == "MAX_GRAVITY_RANGE")
                        val >> MAX_GRAVITY_RANGE;
                    else if (key == "MOUSE_INFLUENCE_RADIUS")
                        val >> MOUSE_INFLUENCE_RADIUS;
                    else if (key == "FRICTION_FACTOR")
                        val >> FRICTION_FACTOR;
                    else if (key == "GRAVITY")
                        val >> GRAVITY;
                }
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open params.txt" << std::endl;
    }
}