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
#include <queue>
#include <algorithm>
#include <ctime> 

// --- ESTRUCTURAS Y ESTADOS (PERSONA 4: ADRIÁN CORREA) ---
struct Node { int x, z; bool operator==(const Node& other) const { return x == other.x && z == other.z; } }; [cite:1]
    enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN }; [cite:1]
    GameState currentState = START_SCREEN; [cite:1]

    // VARIABLES GLOBALES IA Y JUGABILIDAD
    glm::vec3 slenderPos(54.0f, 0.0f, 54.0f); [cite:1]
    float currentSlenderSpeed = 2.5f; [cite:1]
    bool slenderIsVisible = true; [cite:1]
    bool isDyingAnimation = false; [cite:1]
    float dyingTimer = 0.0f; [cite:1]
    bool hasKey = false; [cite:1]
    glm::vec3 keyPos(15.0f, 0.0f, 5.0f); [cite:1]
    std::vector<Node> currentPath; [cite:1]
    float pathUpdateTimer = 0.0f; [cite:1]
    std::vector<Model*> slenderAnimation; [cite:1]

    void stopMusic() { PlaySound(NULL, NULL, 0); }[cite:1]

    // --- FUNCIÓN PATHFINDING BFS ---
    std::vector<Node> findPathBFS(Node start, Node target, int maze[22][22]) {
    std::vector<Node> path; if (start == target) return path;
    Node parents[22][22];
    for (int i = 0; i < 22; i++) for (int j = 0; j < 22; j++) parents[i][j] = { -1,-1 };
    std::queue<Node> q; q.push(start); parents[start.z][start.x] = start;
    bool found = false; int dX[] = { 0,0,-1,1 }; int dZ[] = { -1,1,0,0 };
    while (!q.empty()) {
        Node curr = q.front(); q.pop();
        if (curr == target) { found = true; break; }
        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dX[i], nz = curr.z + dZ[i];
            if (nx >= 0 && nx < 22 && nz >= 0 && nz < 22 && maze[nz][nx] != 1 && parents[nz][nx].x == -1) {
                parents[nz][nx] = curr; q.push({ nx,nz });
            }
        }
    }
    if (found) { Node c = target; while (!(c == start)) { path.push_back(c); c = parents[c.z][c.x]; } std::reverse(path.begin(), path.end()); }
    return path;
}[cite:1]

int main() {
    // Inicialización de GLFW/GLAD...
    // Carga de modelos y texturas... (Commits anteriores)

    // AUDIO INICIAL
    PlaySound(TEXT("audio/horror.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); [cite:1]

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = (float)glfwGetTime();
            float deltaTime = 0.016f; // Aproximado

            if (currentState == PLAYING) {
                // IA: MOVIMIENTO BFS
                pathUpdateTimer += deltaTime;
                if (pathUpdateTimer > 0.2f) {
                    Node s = { (int)round(slenderPos.x / 3.0f), (int)round(slenderPos.z / 3.0f) };
                    Node t = { (int)round(camera.Position.x / 3.0f), (int)round(camera.Position.z / 3.0f) };
                    // currentPath = findPathBFS(s, t, mazeData);
                    pathUpdateTimer = 0.0f;
                }

                // IA: SCREAMER Y MUERTE
                if (!isDyingAnimation && glm::distance(camera.Position, slenderPos) < 2.5f) {
                    isDyingAnimation = true; stopMusic();
                    PlaySound(TEXT("audio/jumpscare.wav"), NULL, SND_FILENAME | SND_ASYNC); [cite:1]
                }
            }

            // UI: PANTALLAS FINALES
            if (currentState == GAME_OVER || currentState == WIN) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); [cite:1]
                    // Dibujar quad con texEnd o texWin... [cite: 1]
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    stopMusic();
    return 0;
}