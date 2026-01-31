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

// --- LÓGICA PERSONA 4 (ADRIAN CORREA) ---
enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN };
GameState currentState = START_SCREEN;

// IA Slender
glm::vec3 slenderPos(18.0f * 3.0f, 0.0f, 18.0f * 3.0f);
bool slenderIsVisible = true;
std::vector<Model*> slenderAnimation;

void stopMusic() { PlaySound(NULL, NULL, 0); }

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
Camera camera(glm::vec3(1.0f, 10.0f, 1.0f));

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SLENDERMAN", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    // Carga de modelos
    stbi_set_flip_vertically_on_load(false);
    for (int i = 0; i <= 10; i++) {
        std::string path = "model/slender/run" + std::to_string(i) + ".glb";
        slenderAnimation.push_back(new Model(path.c_str()));
    }
    stbi_set_flip_vertically_on_load(true);

    Shader slenderShader("shaders/slender.vs", "shaders/slender.fs");
    PlaySound(TEXT("audio/horror.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- RENDERIZADO SLENDER CON LOOKAT (PERSONA 4) ---
        if (slenderIsVisible) {
            slenderShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            slenderShader.setMat4("projection", projection);
            slenderShader.setMat4("view", view);

            // 1. Calcular el ángulo para que mire al jugador
            // Usamos atan2 con la diferencia de posiciones en X y Z
            float angle = atan2(camera.Position.x - slenderPos.x, camera.Position.z - slenderPos.z);

            // 2. Aplicar transformaciones
            glm::mat4 model = glm::translate(glm::mat4(1.0f), slenderPos);
            model = glm::rotate(model, angle, glm::vec3(0, 1, 0)); // Rotación sobre el eje Y
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Corregir orientación del modelo original
            model = glm::scale(model, glm::vec3(0.0045f));
            slenderShader.setMat4("model", model);

            int currentFrameIdx = (int)(glfwGetTime() * 15.0f) % slenderAnimation.size();
            slenderAnimation[currentFrameIdx]->Draw(slenderShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}