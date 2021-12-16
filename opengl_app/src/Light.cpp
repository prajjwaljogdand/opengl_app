
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream> 
#include <string> 
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h" 
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float up = -3.0f;
float right = 0;
float x = 1.0;
float y = 1.0;
float z = 1.0;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;






int main(void)
{
    GLFWwindow* window;
    glewInit();
    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "opengl_app", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


   
    

    std::vector<float> positions = {
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //0
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //1
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //3

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //5
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //7


        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //8
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //9
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //10
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //11

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //12
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //13
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //14
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //15

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //16
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //17
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //18
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //19

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //20
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //21 
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //22
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //23
    };

    std::vector<unsigned int> indices = {
        0,1,2,
    2,3,0,

    4,5,6,
    6,7,4,

    8,9,10,
    10,11,8,

    12,13,14,
    14,15,12,

    16,17,18,
    18,19,16,

    20,21,22,
    22,23,20
    };

   
    Shader shader("res/shaders/LightObject.shader");
    Shader Lightshader("res/shaders/LightSource.shader");
    VertexArray va, val;
    VertexBuffer vb(positions, 24 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    shader.Bind();
    shader.SetUniform4f("objectColor", 1.0f, 0.5f, 0.31f, 1.0f);
    shader.SetUniform4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

    val.AddBuffer(vb, layout);
    Lightshader.Bind();
    Renderer renderer;
    IndexBuffer ib(indices, 36);
    

    /* Loop until the user closes the window */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Lightshader.Unbind();
    shader.Unbind();
    
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processInput(window);
        renderer.Clear();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        float angle = 20.0f;
    



        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::rotate(model2, (float)glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::translate(model2, glm::vec3(1.0f,0.0f, 0.0f));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 mvp = projection * view * model;
        glm::mat4 mvp2 = projection * view * model2;
        Lightshader.Bind();
        Lightshader.SetUniformMat4f("u_MVP2", mvp2);
        renderer.Draw(val, ib, Lightshader);

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);

      
      
       

     

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }







    glfwTerminate();

    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        x += 0.05;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        y += 0.5;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        z += 0.05;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        up -= 0.05;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        up += 0.05;


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


}