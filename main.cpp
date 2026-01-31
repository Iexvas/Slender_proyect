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

// --- LÓGICA PERSONA 4 (ADRIAN CORREA): MUERTE Y SCREAMER ---
enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN };
GameState currentState = START_SCREEN;

bool isDyingAnimation = false;
float dyingTimer = 0.0f;
bool slenderIsVisible = true;
glm::vec3 slenderPos(18.0f * 3.0f, 0.0f, 18.0f * 3.0f);

void stopMusic() { PlaySound(NULL, NULL, 0); }

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
Camera camera(glm::vec3(1.0f, 0.5f, 1.0f));

int main() {
    // ... (Inicializaciones y cargas de commits anteriores) ...

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = 0.016f;

        // --- LÓGICA DE MUERTE (PERSONA 4) ---
        // 1. Detección de colisión (distancia < 2.5 unidades)
        if (!isDyingAnimation && slenderIsVisible && glm::distance(camera.Position, slenderPos) < 2.5f) {
            isDyingAnimation = true;
            dyingTimer = 0.0f;

            stopMusic();
            // Reproducir Jumpscare inmediatamente
            PlaySound(TEXT("audio/jumpscare.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        }

        // 2. Animación de Zoom y bloqueo de vista
        if (isDyingAnimation) {
            dyingTimer += deltaTime;
            glm::vec3 headPos = slenderPos + glm::vec3(0.0f, 1.75f, 0.0f); // Objetivo: Ojos de Slender
            glm::vec3 direction = glm::normalize(headPos - camera.Position);

            // Zoom hacia la cara
            if (glm::distance(camera.Position, headPos) > 0.8f) {
                camera.Position += direction * 1.5f * deltaTime;
            }

            // Forzar rotación de cámara hacia Slender
            camera.Yaw = glm::degrees(atan2(direction.z, direction.x));
            camera.Pitch = glm::degrees(asin(direction.y));

            if (dyingTimer > 2.5f) {
                currentState = GAME_OVER;
                isDyingAnimation = false;
            }
        }

        // Renderizado...
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}