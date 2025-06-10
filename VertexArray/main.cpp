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

// 버텍스 구조체
struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

GLuint vao = 0, vbo = 0, ebo = 0;
size_t indexCount = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void setupVAO() {
	//prepare vertex data
    std::vector<Vertex> vertices(gPositions.size());
    for (size_t i = 0; i < gPositions.size(); ++i) {
        vertices[i].pos = glm::vec3(gPositions[i].x, gPositions[i].y, gPositions[i].z);
        vertices[i].normal = glm::vec3(gNormals[i].x, gNormals[i].y, gNormals[i].z);
    }
	//prepare index data
    std::vector<unsigned int> indices;
    indices.reserve(gTriangles.size() * 3);
    for (const auto& tri : gTriangles) {
        indices.push_back(tri.indices[0]);
        indices.push_back(tri.indices[1]);
        indices.push_back(tri.indices[2]);
    }
    indexCount = indices.size();

	// create VAO, VBO, EBO 
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//position
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    
    //normal
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
}

void drawVAO() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);

	//load bunny mesh
    load_mesh("bunny.obj");

    // VAO/VBO/EBO
    setupVAO();

    //shading parameter
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

        //matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

        glTranslatef(0.1f, -1.0f, -1.5f);
        glScalef(10.0f, 10.0f, 10.0f);

		//start timer
        start_timing();

		// VAO rendering
        drawVAO();

		//stop timer and calculate FPS
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

	//release resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}