#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <random>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int POINT_SIZE = 1;
const int NUM_POINTS = 500;
const float MAX_VELOCITY = 10.0;
const float BASE_GRAVITY_CONSTANT = 0.0008;
const float MAX_GRAVITY_RANGE = 150.0;
const float MOUSE_INFLUENCE_RADIUS = 50.0;  // Radio de influencia del clic del ratón
const float FRICTION_FACTOR = 0.2; // Factor de fricción 

struct Point {
    float x;
    float y;
    float xVelocity;
    float yVelocity;
    float mass;
    SDL_Color color;
};

float calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

float calculateGravityForce(const Point& point, const Point& otherPoint) {
    float distance = calculateDistance(point.x, point.y, otherPoint.x, otherPoint.y);
    float rangeFactor = std::max(1.0 - distance / MAX_GRAVITY_RANGE, 0.0);
    return BASE_GRAVITY_CONSTANT * rangeFactor;
}

void applyGravity(Point& point, const Point& otherPoint) {
    float force = calculateGravityForce(point, otherPoint);
    
    // Aplicar la fuerza de gravedad
    float angle = atan2(otherPoint.y - point.y, otherPoint.x - point.x);
    point.xVelocity += force * cos(angle);
    point.yVelocity += force * sin(angle);
}

void limitVelocity(Point& point) {
    float velocityMagnitude = std::sqrt(std::pow(point.xVelocity, 2) + std::pow(point.yVelocity, 2));

    if (velocityMagnitude > MAX_VELOCITY) {
        float scale = MAX_VELOCITY / velocityMagnitude;
        point.xVelocity *= scale;
        point.yVelocity *= scale;
    }
}

bool checkCollision(const Point& point1, const Point& point2) {
    float distance = calculateDistance(point1.x, point1.y, point2.x, point2.y);
    return distance < POINT_SIZE;
}

void handleCollision(Point& point1, Point& point2) {
    // Calcular las componentes de velocidad relativa
    float relativeVelocityX = point2.xVelocity - point1.xVelocity;
    float relativeVelocityY = point2.yVelocity - point1.yVelocity;

    // Calcular el vector de posición relativa
    float relativeX = point2.x - point1.x;
    float relativeY = point2.y - point1.y;

    // Calcular la distancia entre los centros de las partículas
    float distance = std::sqrt(relativeX * relativeX + relativeY * relativeY);

    // Calcular las componentes de dirección normalizada
    float normalX = relativeX / distance;
    float normalY = relativeY / distance;

    // Calcular la proyección de las velocidades relativas en la dirección normal
    float velocityAlongNormal = relativeVelocityX * normalX + relativeVelocityY * normalY;

    // Si las partículas se están acercando
    if (velocityAlongNormal < 0) {
        // Calcular e intercambiar las velocidades a lo largo de la dirección normal
        float restitution = 0.8;  // Coeficiente de restitución (ajusta según sea necesario)
        float impulse = -(1 + restitution) * velocityAlongNormal / (1 / point1.mass + 1 / point2.mass);

        point1.xVelocity -= (impulse / point1.mass) * normalX;
        point1.yVelocity -= (impulse / point1.mass) * normalY;

        point2.xVelocity += (impulse / point2.mass) * normalX;
        point2.yVelocity += (impulse / point2.mass) * normalY;

        // Aplicar fricción a las velocidades después de la colisión
        point1.xVelocity *= FRICTION_FACTOR;
        point1.yVelocity *= FRICTION_FACTOR;

        point2.xVelocity *= FRICTION_FACTOR;
        point2.yVelocity *= FRICTION_FACTOR;
    }
}

// Nuevas líneas para generar posiciones iniciales con distribuciones aleatorias
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> disX(0, SCREEN_WIDTH - POINT_SIZE);
std::uniform_int_distribution<int> disY(0, SCREEN_HEIGHT - POINT_SIZE);
std::uniform_real_distribution<float> disVelX(-4.0, 4.0);
std::uniform_real_distribution<float> disVelY(-4.0, 4.0);

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no pudo inicializarse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Colisiones entre Puntos", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("La ventana no pudo crearse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("El renderizador no pudo crearse! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    std::vector<Point> points;

    for (int i = 0; i < NUM_POINTS; ++i) {
        Point point;
        point.x = disX(gen);
        point.y = disY(gen);
        point.xVelocity = disVelX(gen);
	    point.yVelocity = disVelY(gen);
        point.color = {255, 255, 255, 255};
        points.push_back(point);
        point.mass = 1.0;
    }

    bool quit = false;
    bool leftMousePressed = false;
    bool rightMousePressed = false;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMousePressed = true;
            } else if (e.button.button == SDL_BUTTON_RIGHT) {
                rightMousePressed = true;
            }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    leftMousePressed = false;
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    rightMousePressed = false;
                }
            }
	    }

	    // Aplicar la lógica del clic del ratón si está presionado
        if (rightMousePressed || leftMousePressed) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            for (auto& point : points) {
                float distanceToMouse = calculateDistance(point.x, point.y, mouseX, mouseY);
                if (distanceToMouse <= MOUSE_INFLUENCE_RADIUS) {
                    // Calcula la velocidad proporcional a la distancia
                    float influenceFactor = MOUSE_INFLUENCE_RADIUS / (distanceToMouse + 1.0);
                    float angleToMouse = atan2(mouseY - point.y, mouseX - point.x);
                    if(leftMousePressed){
                        point.xVelocity -= influenceFactor * cos(angleToMouse);
                        point.yVelocity -= influenceFactor * sin(angleToMouse);
                    }else if(rightMousePressed){
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

        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                applyGravity(points[i], points[j]);
                applyGravity(points[j], points[i]);
            }
        }

        for (auto& point : points) {
            limitVelocity(point);
        }

        for (auto& point : points) {
            point.x += point.xVelocity;
            point.y += point.yVelocity;

            if (point.x < 0 || point.x > SCREEN_WIDTH - POINT_SIZE) {
                point.xVelocity = -point.xVelocity;
            }
            if (point.y < 0 || point.y > SCREEN_HEIGHT - POINT_SIZE) {
                point.yVelocity = -point.yVelocity;
            }
        }

        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                if (checkCollision(points[i], points[j])) {
		    if(points[j].color.g > 0){
			if(points[j].color.b != 1){
			    points[j].color.b -= 1;
			}else points[j].color.g -= 1;
		    }
		    if(points[i].color.g > 0){
                        if(points[i].color.b != 1){
                            points[i].color.b -= 1;
                        }else points[i].color.g -= 1;
                    }

       	            // Manejar la colisión, ajustando la posición para evitar la superposición
                    float overlap = POINT_SIZE - calculateDistance(points[i].x, points[i].y, points[j].x, points[j].y);
                    float angle = atan2(points[i].y - points[j].y, points[i].x - points[j].x);

                    // Separar los puntos ajustando sus posiciones
                    points[i].x += 0.5 * overlap * cos(angle);
                    points[i].y += 0.5 * overlap * sin(angle);
                    points[j].x -= 0.5 * overlap * cos(angle);
                    points[j].y -= 0.5 * overlap * sin(angle);

                    std::swap(points[i].xVelocity, points[j].xVelocity);
                    std::swap(points[i].yVelocity, points[j].yVelocity);
                    handleCollision(points[i], points[j]);
                }
            }
        }

        for (const auto& point : points) {
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
