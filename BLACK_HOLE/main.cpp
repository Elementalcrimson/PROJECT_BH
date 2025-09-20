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
    float speed;
    float alpha;
    float angle;       // for orbiting
    bool orbiting;     // state flag
};

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) {
        std::cerr << "FAILED TO INITIALIZE GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Black Hole Rays", nullptr, nullptr);
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

    // Orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cx = WIDTH / 2.0f;
    float cy = HEIGHT / 2.0f;
    float radius = 50.0f;

    std::vector<Ray> rays;
    const int NUM_RAYS = 150;
    for (int i = 0; i < NUM_RAYS; ++i) {
        rays.push_back({0.0f, static_cast<float>(rand() % HEIGHT),
                        2.0f + static_cast<float>(rand() % 30)/10.0f,
                        1.0f, 0.0f, false});
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw black hole
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for (int i = 0; i <= 100; ++i) {
                float angle = i * 2.0f * 3.1415926f / 100;
                glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
            }
        glEnd();

        // Draw rays
        glBegin(GL_LINES);
        for (auto &r : rays) {
            if (!r.orbiting) {
                // Move towards black hole
                float dx = cx - r.x;
                float dy = cy - r.y;
                float dist = std::sqrt(dx*dx + dy*dy);

                // Check if ray reaches black hole circumference
                if (dist <= radius) {
                    r.orbiting = true;
                    r.angle = atan2(dy, dx); // current angle for orbit
                    dist = radius;
                    r.x = cx + cos(r.angle) * radius;
                    r.y = cy + sin(r.angle) * radius;
                } else {
                    float move_factor = r.speed / dist;
                    r.x += dx * move_factor;
                    r.y += dy * move_factor;
                }
            } else {
                // Orbit around black hole
                float orbit_speed = 0.05f; // adjust for faster/slower orbit
                r.angle += orbit_speed;
                r.x = cx + cos(r.angle) * radius;
                r.y = cy + sin(r.angle) * radius;
            }

            glColor4f(1.0f, 1.0f, 1.0f, r.alpha);
            glVertex2f(r.x, r.y);
            glVertex2f(r.x + 5.0f*cos(r.angle), r.y + 5.0f*sin(r.angle));
            r.alpha -= 0.003f; // fade slowly
        }
        glEnd();

        // Reset rays if offscreen or fully faded
        for (auto &r : rays) {
            if (r.alpha <= 0.0f || r.x < 0 || r.x > WIDTH || r.y < 0 || r.y > HEIGHT) {
                r.x = 0.0f;
                r.y = static_cast<float>(rand() % HEIGHT);
                r.speed = 2.0f + static_cast<float>(rand() % 30)/10.0f;
                r.alpha = 1.0f;
                r.orbiting = false;
                r.angle = 0.0f;
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
