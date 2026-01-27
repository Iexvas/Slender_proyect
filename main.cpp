#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h> // Asegurate de tener este header
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float BLOCK_SIZE = 3.0f;
float deltaTime = 0.0f; 
float lastFrame = 0.0f;

// --- DATOS DEL MAPA (1=Pared, 0=Camino) ---
const int MAZE_WIDTH = 22;
const int MAZE_HEIGHT = 22;
int mazeData[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 5, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 4, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Vertices simples (Cubo)
float cubeVertices[] = { -0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f, 0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,1.0f,0.0f, 0.5f,0.5f,-0.5f,0.0f,0.0f,-1.0f,1.0f,1.0f, 0.5f,0.5f,-0.5f,0.0f,0.0f,-1.0f,1.0f,1.0f, -0.5f,0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,1.0f, -0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f, -0.5f,-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f, 0.5f,-0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,0.0f, 0.5f,0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,1.0f, 0.5f,0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,1.0f, -0.5f,0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,1.0f, -0.5f,-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f, -0.5f,0.5f,0.5f,-1.0f,0.0f,0.0f,1.0f,0.0f, -0.5f,0.5f,-0.5f,-1.0f,0.0f,0.0f,1.0f,1.0f, -0.5f,-0.5f,-0.5f,-1.0f,0.0f,0.0f,0.0f,1.0f, -0.5f,-0.5f,-0.5f,-1.0f,0.0f,0.0f,0.0f,1.0f, -0.5f,-0.5f,0.5f,-1.0f,0.0f,0.0f,0.0f,0.0f, -0.5f,0.5f,0.5f,-1.0f,0.0f,0.0f,1.0f,0.0f, 0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f, 0.5f,0.5f,-0.5f,1.0f,0.0f,0.0f,1.0f,1.0f, 0.5f,-0.5f,-0.5f,1.0f,0.0f,0.0f,0.0f,1.0f, 0.5f,-0.5f,-0.5f,1.0f,0.0f,0.0f,0.0f,1.0f, 0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,0.0f,0.0f, 0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f, -0.5f,-0.5f,-0.5f,0.0f,-1.0f,0.0f,0.0f,1.0f, 0.5f,-0.5f,-0.5f,0.0f,-1.0f,0.0f,1.0f,1.0f, 0.5f,-0.5f,0.5f,0.0f,-1.0f,0.0f,1.0f,0.0f, 0.5f,-0.5f,0.5f,0.0f,-1.0f,0.0f,1.0f,0.0f, -0.5f,-0.5f,0.5f,0.0f,-1.0f,0.0f,0.0f,0.0f, -0.5f,-0.5f,-0.5f,0.0f,-1.0f,0.0f,0.0f,1.0f, -0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f, 0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,1.0f,1.0f, 0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,0.0f, 0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,1.0f,0.0f, -0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f, -0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f };

int main()
{
    // Setup Basico (Commit 1)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Slender: Final Nightmare", NULL, NULL);
    if (window == NULL) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { return -1; }

    glEnable(GL_DEPTH_TEST); // Importante para 3D

    // --- NUEVO EN COMMIT 2: SHADERS Y BUFFER ---
    Shader ourShader("shaders/shader_B2T3.vs", "shaders/shader_B2T3.fs");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Atributos: Posicion (0), Normal (1), Textura (2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --- RENDER LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // Renderizar Mapa (Sin Camara aun)
        glBindVertexArray(VAO);
        for (int z = 0; z < MAZE_HEIGHT; z++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                if (mazeData[z][x] == 1) { 
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x * BLOCK_SIZE, 0.0f, z * BLOCK_SIZE));
                    model = glm::scale(model, glm::vec3(BLOCK_SIZE, 4.0f, BLOCK_SIZE));
                    ourShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}