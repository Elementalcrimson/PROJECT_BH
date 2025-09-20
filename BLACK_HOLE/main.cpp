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

    glPointSize(2.0f); // size of ray points if needed
    glLineWidth(2.0f); // width of rays

    // Orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Create rays
    std::vector<Ray> rays;
    const int NUM_RAYS = 150;
    for (int i = 0; i < NUM_RAYS; ++i) {
        rays.push_back({0.0f, static_cast<float>(rand() % HEIGHT), 2.0f + static_cast<float>(rand() % 30)/10.0f, 1.0f});
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw red circle (black hole)
        float cx = WIDTH / 2.0f;
        float cy = HEIGHT / 2.0f;
        float radius = 50.0f;
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for (int i = 0; i <= 100; ++i) {
                float angle = i * 2.0f * 3.1415926f / 100;
                glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
            }
        glEnd();

        // Draw rays as line segments
        glBegin(GL_LINES);
        for (auto &r : rays) {
            glColor4f(1.0f, 1.0f, 1.0f, r.alpha);
            glVertex2f(r.x, r.y);
            glVertex2f(r.x + 20.0f, r.y); // line segment length
            r.x += r.speed;
            r.alpha -= 0.005f; // fade speed
        }
        glEnd();

        // Reset rays when off-screen or fully faded
        for (auto &r : rays) {
            if (r.x > WIDTH || r.alpha <= 0.0f) {
                r.x = 0.0f;
                r.y = static_cast<float>(rand() % HEIGHT);
                r.alpha = 1.0f;
                r.speed = 2.0f + static_cast<float>(rand() % 30)/10.0f;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Control speed
        glfwWaitEventsTimeout(0.01); // ~10ms delay
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
