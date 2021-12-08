
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
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>



int main(void)
{
    GLFWwindow* window;
    glewInit();
    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "opengl_app", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);

    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


    {

        float positions[] = {
            0.0f, 0.0f, 0.0f , 0.0f,
             500.0f, 0.0f, 1.0f, 0.0f,
             500.0f,  500.0f, 1.0f , 1.0f,
            0.0f,  500.0f,0.0f ,1.0f
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };


        //enabling blend mode so transperant area will be transparent
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // creating vertexarray 
        VertexArray va; 

        //creating array buffer with providing size and data 
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        
        // creating layout and pushing it 
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        //binding vertexarray with vertex array buffer and layout  
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(-1.0f), glm::vec3(10, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(-1.0f), glm::vec3(10, 0, 0));
        glm::mat4 mvp = proj * view * model;
        

        //passinf fileoath of shaders in shader object 
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color",0.8f, 0.3f, 0.6f, 1.0);
        shader.SetUniformMat4f("u_MVP",mvp);


        float red = 0.0f;
        float increment = 0.05f;

        Texture texture("res/textures/text.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        const char* glsl_version = "#version 130";
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();
        bool show_demo_window = true;
        bool show_another_window = true;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Loop until the user closes the window */
    
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            va.Bind();
            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.3f, 0.6f, 1.0);
           
            renderer.Draw(va, ib, shader);


            if (red > 1.0f) increment = -0.05f;
            else if (red < 0.0f) increment = 0.05f;
            red += increment;
            
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
           
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
        glfwTerminate();
    
    return 0;
}