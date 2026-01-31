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
enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN }; [cite:1]
    GameState currentState = START_SCREEN; [cite:1]

    // IA Slender
    glm::vec3 slenderPos(18.0f * 3.0f, 0.0f, 18.0f * 3.0f); [cite:1]
    bool slenderIsVisible = true; [cite:1]
    std::vector<Model*> slenderAnimation; [cite:1]

    void stopMusic() { PlaySound(NULL, NULL, 0); }[cite:1]

    const unsigned int SCR_WIDTH = 1280; [cite:1]
    const unsigned int SCR_HEIGHT = 720; [cite:1]
    Camera camera(glm::vec3(1.0f, 10.0f, 1.0f)); [cite:1]

    int main() {
    srand(static_cast<unsigned int>(time(NULL))); [cite:1]
        glfwInit();
    // ... (Inicialización de ventana estándar)
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SLENDERMAN", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST); [cite:1]

        // --- CARGA DE MODELOS (PERSONA 4) ---
        stbi_set_flip_vertically_on_load(false); [cite:1]
        for (int i = 0; i <= 10; i++) {
            std::string path = "model/slender/run" + std::to_string(i) + ".glb"; [cite:1]
                slenderAnimation.push_back(new Model(path.c_str())); [cite:1]
        }
    stbi_set_flip_vertically_on_load(true); [cite:1]

        // Shaders (Asumimos que Persona 3 ya los puso en la carpeta)
        Shader slenderShader("shaders/slender.vs", "shaders/slender.fs"); [cite:1]

        PlaySound(TEXT("audio/horror.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); [cite:1]

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = (float)glfwGetTime();
            glClearColor(0.01f, 0.01f, 0.02f, 1.0f); [cite:1]
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); [cite:1]

                // --- RENDERIZADO SLENDER (PERSONA 4) ---
                if (slenderIsVisible) {
                    slenderShader.use();
                    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); [cite:1]
                        glm::mat4 view = camera.GetViewMatrix(); [cite:1]
                        slenderShader.setMat4("projection", projection); [cite:1]
                        slenderShader.setMat4("view", view); [cite:1]

                        glm::mat4 model = glm::translate(glm::mat4(1.0f), slenderPos); [cite:1]
                        model = glm::scale(model, glm::vec3(0.0045f)); [cite:1]
                        slenderShader.setMat4("model", model); [cite:1]

                        int currentFrameIdx = (int)(glfwGetTime() * 15.0f) % slenderAnimation.size(); [cite:1]
                        slenderAnimation[currentFrameIdx]->Draw(slenderShader); [cite:1]
                }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    glfwTerminate();
    return 0;
}