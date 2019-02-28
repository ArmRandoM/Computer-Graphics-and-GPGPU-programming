#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include "build/shader_s.h"
#include "build/data_set.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void fill_verices_color_buffer(float vertices_and_colours[], float z_min, float z_max);
void fill_indices(int* indices);

DataFileReader data_file_reader("DEM_test.dat");

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    //The z_min and z_max values used to compute the color for each vertex
    float z_min = data_file_reader.get_z_min();
    float z_max = data_file_reader.get_z_max();

    //The number of values for the array of the vertices (the dimension )
    int values_number_vertices = (data_file_reader.getNumRows() * data_file_reader.getNumColumns())*6;

    //The number of values for the array of the indices ( the dimension )
    int values_number_indices = ((data_file_reader.getNumRows()-1) * (data_file_reader.getNumColumns()-1))*6;

    //The vertices array contains also the colors for each vertex
    float vertices_and_colours [values_number_vertices];

    //The indices array contains all indices for each triangle
    int indices [values_number_indices];

    //Function that computes the vertices and colours
    fill_verices_color_buffer(vertices_and_colours, z_min, z_max);

    //Function that computes the indices that are used for each triangle
    fill_indices(indices);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_and_colours), vertices_and_colours, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, values_number_vertices*3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void fill_verices_color_buffer(float vertices_and_colours[], float z_min, float z_max){
    int index = 0;
    float minX = data_file_reader.getXllCorner();
    float minY = data_file_reader.getYllCorner();
    for(int i = data_file_reader.getNumRows()-1; i >= 0; i--)
    {
        for(int j = 0; j < data_file_reader.getNumColumns(); j++)
        {
            vertices_and_colours[index] = minX;
            vertices_and_colours[index+1] = minY;
            vertices_and_colours[index+2] = data_file_reader.getVerticesMatrix()[i][j];

            vertices_and_colours[index+3] = (vertices_and_colours[index+2] - z_min) / (z_max - z_min);
            vertices_and_colours[index+4] = (vertices_and_colours[index+2] - z_min) / (z_max - z_min);
            vertices_and_colours[index+5] = (vertices_and_colours[index+2] - z_min) / (z_max - z_min);

            index += 6;
            minX += data_file_reader.getCellSize();
        }
        minX = data_file_reader.getXllCorner();
        minY += data_file_reader.getCellSize();
    }
}

void fill_indices( int indices[])
{
    int current_vertex = 0;
    int index = 0;

    for(int i = 0; i < data_file_reader.getNumRows(); i++)
    {
        for(int j = 0; j < data_file_reader.getNumColumns(); j++)
        {
            if(i < data_file_reader.getNumRows() - 1 && j < data_file_reader.getNumColumns() - 1)
            {
                indices[index] = current_vertex;
                indices[index+1] = current_vertex + 1;
                indices[index+2] = current_vertex + data_file_reader.getNumColumns() + 1;

                indices[index+3] = current_vertex + data_file_reader.getNumColumns() + 1;
                indices[index+4] = current_vertex + data_file_reader.getNumColumns();
                indices[index+5] = current_vertex;

                index += 6;
            }
            current_vertex++;
        }
    }
}
