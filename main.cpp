#define _HAS_STD_BYTE 0
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#define STB_IMAGE_IMPLEMENTATION
#include <learnopengl/stb_image.h>

#include <iostream>
#include <vector>
#include <ctime> 

// --- LÓGICA PERSONA 4: ESTADOS Y AUDIO --- 
enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN };
GameState currentState = START_SCREEN; [cite:1]

void stopMusic() {
    PlaySound(NULL, NULL, 0);
}[cite:1]

// Configuración básica (Arquitectura compartida) 
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
Camera camera(glm::vec3(1.0f, 10.0f, 1.0f)); [cite:1]

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SLENDERMAN: FINAL NIGHTMARE", NULL, NULL);
    if (window == NULL) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // --- LÓGICA PERSONA 4: ACTIVAR AUDIO AMBIENTAL --- 
    PlaySound(TEXT("audio/horror.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); [cite:1]

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = (float)glfwGetTime();

            glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Aquí se mantiene la arquitectura base para que el juego abra
            // Los commits siguientes irán "descomentando" la IA y las reglas

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    stopMusic(); [cite:1]
        glfwTerminate();
    return 0;
}