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

// --- ESTRUCTURAS IA (PERSONA 4) ---
struct Node {
    int x, z;
    bool operator==(const Node& other) const { return x == other.x && z == other.z; }
}; [cite:10]

// --- LÓGICA PERSONA 4 (ADRIAN CORREA) ---
enum GameState { MENU, PLAYING, GAME_OVER, WIN, START_SCREEN };
GameState currentState = START_SCREEN; [cite:10]

// IA Slender y Navegación
glm::vec3 slenderPos(18.0f * 3.0f, 0.0f, 18.0f * 3.0f); [cite:12]
float currentSlenderSpeed = 2.5f; [cite:10]
std::vector<Node> currentPath; [cite:10]
float pathUpdateTimer = 0.0f; [cite:10]
bool slenderIsVisible = true; [cite:10]
std::vector<Model*> slenderAnimation; [cite:10]

// Mapa (Copia del mazeData para el BFS)
const int MAZE_WIDTH = 22; [cite:10]
const int MAZE_HEIGHT = 22; [cite:10]
extern int mazeData[22][22]; // Asumimos que la arquitectura base ya tiene el mapa definido [cite: 10]

// --- FUNCIÓN DE BÚSQUEDA DE CAMINOS BFS ---
std::vector<Node> findPathBFS(Node start, Node target) {
    std::vector<Node> path;
    if (start.x == target.x && start.z == target.z) return path;
    Node parents[22][22];
    for (int i = 0; i < 22; i++) for (int j = 0; j < 22; j++) parents[i][j] = { -1,-1 };

    std::queue<Node> q; q.push(start); parents[start.z][start.x] = start;
    bool found = false; int dX[] = { 0,0,-1,1 }; int dZ[] = { -1,1,0,0 };

    while (!q.empty()) {
        Node curr = q.front(); q.pop();
        if (curr.x == target.x && curr.z == target.z) { found = true; break; }
        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dX[i]; int nz = curr.z + dZ[i];
            if (nx >= 0 && nx < 22 && nz >= 0 && nz < 22) {
                // El Slender no atraviesa paredes (1) ni la fuente (5)
                if (mazeData[nz][nx] != 1 && mazeData[nz][nx] != 5 && parents[nz][nx].x == -1) {
                    [cite:13]
                    parents[nz][nx] = curr; q.push({ nx,nz });
                }
            }
        }
    }
    if (found) {
        Node c = target;
        while (!(c.x == start.x && c.z == start.z)) {
            path.push_back(c); c = parents[c.z][c.x];
        }
        std::reverse(path.begin(), path.end());
    }
    return path; [cite:13]
}

int main() {
    // ... (Inicialización y Carga de Modelos del commit anterior) ...

    while (!glfwWindowShouldClose(window)) {
        float deltaTime = 0.016f; // Simplificado para el ejemplo

        // --- LÓGICA DE PERSECUCIÓN (PERSONA 4) ---
        pathUpdateTimer += deltaTime;
        if (pathUpdateTimer > 0.2f) { // Actualizar ruta cada 0.2 segundos [cite: 16]
            Node s = { (int)round(slenderPos.x / 3.0f), (int)round(slenderPos.z / 3.0f) };
            Node t = { (int)round(camera.Position.x / 3.0f), (int)round(camera.Position.z / 3.0f) };
            currentPath = findPathBFS(s, t);
            pathUpdateTimer = 0.0f;
        }

        if (!currentPath.empty()) {
            glm::vec3 dest((float)currentPath[0].x * 3.0f, 0.0f, (float)currentPath[0].z * 3.0f);
            glm::vec3 dir = glm::normalize(dest - slenderPos);
            slenderPos += dir * currentSlenderSpeed * deltaTime; [cite:16]
                if (glm::distance(slenderPos, dest) < 0.2f) currentPath.erase(currentPath.begin());
        }

        // Renderizado de Slender (con el LookAt del commit anterior)...
        // ...

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}