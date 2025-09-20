#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Ray {
    float x, y;
    float vx, vy; // velocity
    float alpha;
};

const int WIDTH = 800;
const int HEIGHT = 600;
const float BH_RADIUS = 50.0f;
const float G = 500.0f; // attraction strength

void acceleration(float x, float y, float cx, float cy, float &ax, float &ay) {
    float dx = cx - x;
    float dy = cy - y;
    float distSq = dx*dx + dy*dy;
    float dist = std::sqrt(distSq);
    if (dist < BH_RADIUS) dist = BH_RADIUS;
    float force = G / (distSq);
    ax = force * dx / dist;
    ay = force * dy / dist;
}

void rk4(Ray &r, float dt, float cx, float cy) {
    float k1x, k1y, k1vx, k1vy;
    float k2x, k2y, k2vx, k2vy;
    float k3x, k3y, k3vx, k3vy;
    float k4x, k4y, k4vx, k4vy;

    float ax, ay;

    acceleration(r.x, r.y, cx, cy, ax, ay);
    k1vx = ax * dt; k1vy = ay * dt;
    k1x = r.vx * dt; k1y = r.vy * dt;

    acceleration(r.x + 0.5f*k1x, r.y + 0.5f*k1y, cx, cy, ax, ay);
    k2vx = ax * dt; k2vy = ay * dt;
    k2x = (r.vx + 0.5f*k1vx) * dt; k2y = (r.vy + 0.5f*k1vy) * dt;

    acceleration(r.x + 0.5f*k2x, r.y + 0.5f*k2y, cx, cy, ax, ay);
    k3vx = ax * dt; k3vy = ay * dt;
    k3x = (r.vx + 0.5f*k2vx) * dt; k3y = (r.vy + 0.5f*k2vy) * dt;

    acceleration(r.x + k3x, r.y + k3y, cx, cy, ax, ay);
    k4vx = ax * dt; k4vy = ay * dt;
    k4x = (r.vx + k3vx) * dt; k4y = (r.vy + k3vy) * dt;

    r.x += (k1x + 2*k2x + 2*k3x + k4x) / 6.0f;
    r.y += (k1y + 2*k2y + 2*k3y + k4y) / 6.0f;
    r.vx += (k1vx + 2*k2vx + 2*k3vx + k4vx) / 6.0f;
    r.vy += (k1vy + 2*k2vy + 2*k3vy + k4vy) / 6.0f;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) {
        std::cerr << "FAILED TO INITIALIZE GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Black Hole Rays RK4", nullptr, nullptr);
    if (!window) {
        std::cerr << "FAILED TO OPEN WINDOW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "FAILED TO INITIALIZE GLEW\n";
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cx = WIDTH / 2.0f;
    float cy = HEIGHT / 2.0f;

    std::vector<Ray> rays;
    const int NUM_RAYS = 150;
    for (int i = 0; i < NUM_RAYS; ++i) {
        float y = static_cast<float>(rand() % HEIGHT);
        rays.push_back({0.0f, y, 5.0f + static_cast<float>(rand() % 30)/10.0f, 0.0f, 1.0f}); // faster
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw black hole
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i=0;i<=100;i++){
            float angle = i*2.0f*3.1415926f/100;
            glVertex2f(cx + cos(angle)*BH_RADIUS, cy + sin(angle)*BH_RADIUS);
        }
        glEnd();

        glBegin(GL_LINES);
        for (auto &r : rays) {
            float dist = std::hypot(r.x-cx, r.y-cy);
            if (dist <= BH_RADIUS) r.alpha = 0.0f;

            glColor4f(1.0f,1.0f,1.0f,r.alpha);
            glVertex2f(r.x,r.y);
            glVertex2f(r.x - r.vx*5, r.y - r.vy*5);

            rk4(r, 0.1f, cx, cy);

            r.alpha -= 0.0005f; // slower fade
        }
        glEnd();

        for (auto &r : rays) {
            if (r.x > WIDTH || r.alpha <= 0.0f || r.y < 0 || r.y > HEIGHT) {
                r.x = 0.0f;
                r.y = static_cast<float>(rand()%HEIGHT);
                r.vx = 6.0f + static_cast<float>(rand() % 40)/10.0f; // faster
                r.vy = 0.0f;
                r.alpha = 1.0f;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(0.01);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
