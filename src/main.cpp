#include "../include/utils.hpp"
#include <SDL2/SDL.h>
#include <vector>

int main()
{
    int SCREEN_WIDTH = 600;
    int SCREEN_HEIGHT = 400;
    int POINT_SIZE = 1;
    int NUM_POINTS = 500;
    float MAX_VELOCITY = 4.0f;
    float BASE_GRAVITY_CONSTANT = 0.0008f;
    float MAX_GRAVITY_RANGE = 50.0f;
    float MOUSE_INFLUENCE_RADIUS = 50.0f;
    float FRICTION_FACTOR = 0.2f;
    float GRAVITY = 0.0f;

    read_params(SCREEN_WIDTH, SCREEN_HEIGHT, POINT_SIZE, NUM_POINTS, MAX_VELOCITY, BASE_GRAVITY_CONSTANT,
                MAX_GRAVITY_RANGE, MOUSE_INFLUENCE_RADIUS, FRICTION_FACTOR, GRAVITY);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL no pudo inicializarse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("ParticleSim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("La ventana no pudo crearse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("El renderizador no pudo crearse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    std::vector<Point> points;

    for (int i = 0; i < NUM_POINTS; ++i)
    {
        Point point;
        point.x = generate_random_int(0, SCREEN_WIDTH - POINT_SIZE);
        point.y = generate_random_int(0, SCREEN_HEIGHT - POINT_SIZE);
        point.xVelocity = generate_random_float(-1.0, 1.0);
        point.yVelocity = generate_random_float(-1.0, 1.0);
        point.color = {255, 255, 255, 255};
        points.push_back(point);
        point.mass = 1.0;
    }

    bool quit = false;
    bool leftMousePressed = false;
    bool rightMousePressed = false;

    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    leftMousePressed = true;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    rightMousePressed = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    leftMousePressed = false;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    rightMousePressed = false;
                }
            }
        }

        // Aplicar la lógica del clic del ratón si está presionado
        if (rightMousePressed || leftMousePressed)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            for (auto &point : points)
            {
                float distanceToMouse = calculate_distance(point.x, point.y, mouseX, mouseY);
                if (distanceToMouse <= MOUSE_INFLUENCE_RADIUS)
                {
                    // Calcula la velocidad proporcional a la distancia
                    float influenceFactor = MOUSE_INFLUENCE_RADIUS / (distanceToMouse + 1.0);
                    float angleToMouse = atan2(mouseY - point.y, mouseX - point.x);
                    if (leftMousePressed)
                    {
                        point.xVelocity -= influenceFactor * cos(angleToMouse);
                        point.yVelocity -= influenceFactor * sin(angleToMouse);
                    }
                    else if (rightMousePressed)
                    {
                        point.xVelocity += influenceFactor * cos(angleToMouse);
                        point.yVelocity += influenceFactor * sin(angleToMouse);
                    }

                    // Restaurar el color inicial
                    point.color = {255, 255, 255, 255};
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < points.size(); ++i)
        {
            for (size_t j = i + 1; j < points.size(); ++j)
            {
                apply_gravity(points[i], points[j]);
                apply_gravity(points[j], points[i]);
            }
        }

        for (auto &point : points)
        {
            point.yVelocity += GRAVITY;
        }

        for (auto &point : points)
        {
            limit_velocity(point);
        }

        for (auto &point : points)
        {
            point.x += point.xVelocity;
            point.y += point.yVelocity;

            // Corregir la posición para que la partícula permanezca dentro de los límites de la ventana
            if (point.x < 0)
            {
                point.x = 0;
                point.xVelocity = -point.xVelocity; // Invertir la velocidad en x
            }
            else if (point.x > SCREEN_WIDTH - POINT_SIZE)
            {
                point.x = SCREEN_WIDTH - POINT_SIZE;
                point.xVelocity = -point.xVelocity; // Invertir la velocidad en x
            }

            if (point.y < 0)
            {
                point.y = 0;
                point.yVelocity = -point.yVelocity; // Invertir la velocidad en y
            }
            else if (point.y > SCREEN_HEIGHT - POINT_SIZE)
            {
                point.y = SCREEN_HEIGHT - POINT_SIZE;
                point.yVelocity = -point.yVelocity; // Invertir la velocidad en y
            }
        }

        for (size_t i = 0; i < points.size(); ++i)
        {
            for (size_t j = i + 1; j < points.size(); ++j)
            {
                if (check_collision(points[i], points[j]))
                {
                    if (points[j].color.g > 0)
                    {
                        if (points[j].color.b != 1)
                        {
                            points[j].color.b -= 1;
                        }
                        else
                            points[j].color.g -= 1;
                    }
                    if (points[i].color.g > 0)
                    {
                        if (points[i].color.b != 1)
                        {
                            points[i].color.b -= 1;
                        }
                        else
                            points[i].color.g -= 1;
                    }

                    // Manejar la colisión, ajustando la posición para evitar la superposición
                    float overlap = POINT_SIZE - calculate_distance(points[i].x, points[i].y, points[j].x, points[j].y);
                    float angle = atan2(points[i].y - points[j].y, points[i].x - points[j].x);

                    // Separar los puntos ajustando sus posiciones
                    points[i].x += 0.5 * overlap * cos(angle);
                    points[i].y += 0.5 * overlap * sin(angle);
                    points[j].x -= 0.5 * overlap * cos(angle);
                    points[j].y -= 0.5 * overlap * sin(angle);

                    std::swap(points[i].xVelocity, points[j].xVelocity);
                    std::swap(points[i].yVelocity, points[j].yVelocity);
                    handle_collision(points[i], points[j]);
                }
            }
        }

        for (const auto &point : points)
        {
            SDL_SetRenderDrawColor(renderer, point.color.r, point.color.g, point.color.b, point.color.a);
            SDL_Rect pointRect = {static_cast<int>(point.x), static_cast<int>(point.y), POINT_SIZE, POINT_SIZE};
            SDL_RenderFillRect(renderer, &pointRect);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
