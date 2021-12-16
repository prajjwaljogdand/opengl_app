
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
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;




    float positions[] = {
      -0.5, 0.0, 0.5, 1.0 , 0.0 , 0.0, //0
       0.5, 0.0, 0.5, 0.0 , 1.0 , 0.0, //1
       0.0, 0.0,-0.5, 0.0 , 0.0 , 1.0, //2
       0.0, 0.75, 0.0, 1.0 , 1.0 , 1.0, //3
       0.0, -0.75, 0.0, 0.0 , 0.0 , 0.0, //4
    };

    unsigned int indices[] = {
       0,1,2,
       0,1,3,
       1,2,3,
       2,0,3,
       0,1,4,
       1,2,4,
       2,0,4

    };


    //enabling blend mode so transperant area will be transparent


    // creating vertexarray 
    VertexArray va;
    //creating array buffer with providing size and data 
    VertexBuffer vb(positions, 5 * 6 * sizeof(float));
    // creating layout and pushing it 
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);

    //binding vertexarray with vertex array buffer and layout  
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 21);

    //passinf fileoath of shaders in shader object 
    Shader shader("res/shaders/Basic2.shader");
    shader.Bind();
    Renderer renderer;

  
    /* Loop until the user closes the window */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        renderer.Clear();
        shader.Bind();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
       


            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            float angle = 20.0f * glfwGetTime();
            model = glm::rotate(model, (float)glm::radians(angle) , glm::vec3(1.0f, 1.0f, 1.0f));

            glm::mat4 mvp = projection * view * model;
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