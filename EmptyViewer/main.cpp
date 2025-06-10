#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include "load_mesh.h"     // gPositions, gNormals, gTriangles
#include "frame_timer.h"   // start_timing, stop_timing

using namespace std;
using namespace glm;

int Width = 1280, Height = 1280;
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    window = glfwCreateWindow(Width, Height, "OpenGL Bunny", NULL, NULL);
    if (!window) {
        glfwTerminate(); return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // Back-face culling off

	// activate lighting
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);

	//load bunny mesh
    load_mesh("bunny.obj");

    //light
    GLfloat ambientGlobal[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientGlobal);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	// light direction  (1, 1, 1) normalized
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    float lightPos[] = { lightDirection.x, lightDirection.y, lightDirection.z, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//material
    GLfloat mat_ka_kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ka_kd);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ka_kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_ks);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

	//init timer
    init_timer();
    int frameCount = 0;
    double totalTime = 0.0;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //transform matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

        glTranslatef(0.1f, -1.0f, -1.5f);
        glScalef(10.0f, 10.0f, 10.0f);

		// start the timer
        start_timing();

        // Immediate Mode
        glBegin(GL_TRIANGLES);
        for (const auto& tri : gTriangles) {
            for (int i = 0; i < 3; ++i) {
                const auto& n = gNormals[tri.indices[i]];
                const auto& v = gPositions[tri.indices[i]];
                glNormal3f(n.x, n.y, n.z);
                glVertex3f(v.x, v.y, v.z);
            }
        }
        glEnd();

		//stop the timer and calculate FPS
        double elapsed = stop_timing();
        totalTime += elapsed;
        frameCount++;
        double fps = frameCount / totalTime;

        char title[128];
        sprintf_s(title, "OpenGL Bunny - %.2f FPS", fps);
        glfwSetWindowTitle(window, title);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}