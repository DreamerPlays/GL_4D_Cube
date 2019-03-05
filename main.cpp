#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <math.h>
#include <malloc.h>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
#include "fps.h"

#include "renderer.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "vertexArray.h"
#include "Robject.h"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::mat4 rotXW(float angle) {
  return glm::mat4(glm::cos(angle),0.0f,0.0f,glm::sin(angle),
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   -glm::sin(angle),0.0f,0.0f,glm::cos(angle));
}

glm::mat4 rotYW(float angle) {
  return glm::mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,glm::cos(angle),0.0f,-glm::sin(angle),
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,glm::sin(angle),0.0f,glm::cos(angle));
}

glm::mat4 rotZW(float angle) {
  return glm::mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,glm::cos(angle),-glm::sin(angle),
                   0.0f,0.0f,glm::sin(angle),glm::cos(angle));
}


int main(int argc, char** argv)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;


  /* Create a windowed to know whats the highest opengl version supported*/
  GLFWwindow* context = glfwCreateWindow(1, 1, "contextgetter", NULL, NULL);
  if (!context)
  {
    glfwTerminate();
    return -1;
  }
  glfwSetWindowPos(context, 0, 0);

  /* Make the window's context current */
  glfwMakeContextCurrent(context);

  std::string version((char*)glGetString(GL_VERSION));
  std::cout << "highest context: " << version.at(0) << "." << version.at(2) << std::endl;

  glfwDestroyWindow(context);

  const int maj = (const int)version.at(0) - 48;
  const int min = (const int)version.at(2) - 48;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // 4xMSAA
  glfwWindowHint(GLFW_SAMPLES, 4);


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(800, 800, "GL_4D_Cube", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
    std::cout << "Error initialilzing GLEW!" << std::endl;


  std::cout << glGetString(GL_VERSION) << std::endl;


  float positions[] = {
    -1.0f, -1.0f, -1.0f,  1.0f, //0
     1.0f, -1.0f, -1.0f,  1.0f, //1
     1.0f,  1.0f, -1.0f,  1.0f, //2
    -1.0f,  1.0f, -1.0f,  1.0f, //3
    -1.0f, -1.0f,  1.0f,  1.0f, //4
     1.0f, -1.0f,  1.0f,  1.0f, //5
     1.0f,  1.0f,  1.0f,  1.0f, //6
    -1.0f,  1.0f,  1.0f,  1.0f, //7
    -1.0f, -1.0f, -1.0f, -1.0f, //8
     1.0f, -1.0f, -1.0f, -1.0f, //9
     1.0f,  1.0f, -1.0f, -1.0f, //10
    -1.0f,  1.0f, -1.0f, -1.0f, //11
    -1.0f, -1.0f,  1.0f, -1.0f, //12
     1.0f, -1.0f,  1.0f, -1.0f, //13
     1.0f,  1.0f,  1.0f, -1.0f, //14
    -1.0f,  1.0f,  1.0f, -1.0f  //15
  };

  unsigned int indecies[] = {
    0, 1,   //first cube
    1, 2,   //first cube
    2, 3,   //first cube
    3, 0,   //first cube
    4, 5,   //first cube
    5, 6,   //first cube
    6, 7,   //first cube
    7, 4,   //first cube
    0, 4,   //first cube
    1, 5,   //first cube
    2, 6,   //first cube
    3, 7,   //first cube
    8, 9,   //second cube
    9, 10,  //second cube
    10, 11, //second cube
    11, 8,  //second cube
    12, 13, //second cube
    13, 14, //second cube
    14, 15, //second cube
    15, 12, //second cube
    8, 12,  //second cube
    9, 13,  //second cube
    10, 14, //second cube
    11, 15, //second cube
    0,8,    //first to second cube
    1,9,    //first to second cube
    2,10,   //first to second cube
    3,11,   //first to second cube
    4,12,   //first to second cube
    5,13,   //first to second cube
    6,14,   //first to second cube
    7,15,    //first to second cube
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 //Points
  };

  vertexBufferLayout* layout = new vertexBufferLayout;
  layout->push<float>(4);
  Robject* cube = new Robject(positions, 4 * 16, indecies, 2 * 32 + 16, layout);

  shader * circlePoint = new shader();
  circlePoint->addShader(GL_VERTEX_SHADER, "./shaders/vertex.shader", SH_FILEPATH);
  circlePoint->addShader(GL_FRAGMENT_SHADER, "./shaders/fragmentCircle.shader", SH_FILEPATH);
  circlePoint->compileShader();

  shader* standard = new shader();
  standard->addShader(GL_VERTEX_SHADER, "./shaders/vertex.shader", SH_FILEPATH);
  standard->addShader(GL_FRAGMENT_SHADER, "./shaders/fragment.shader", SH_FILEPATH);
  standard->compileShader();
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0,0,-5),glm::vec3(0,0,0),glm::vec3(0,1,0));
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f / 1.0f, 0.1f, 100.0f);
  glm::mat4 M3D = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix * scalingMatrix; 
  standard->bind();
  standard->setUniformMat4fv("m_3d", M3D);
  circlePoint->bind();
  circlePoint->setUniformMat4fv("m_3d", M3D);

  //4D Projection
  float angle = glm::radians(0.0f);

  glm::mat4 rot4D = rotXW(angle);

  standard->bind();
  standard->setUniformMat4fv("m_rot4d", rot4D);
  circlePoint->bind();
  circlePoint->setUniformMat4fv("m_rot4d", rot4D);

  float alpha = 1.0f;

  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  fps* tfps = new fps();
  glfwSwapInterval(1);
  while (!glfwWindowShouldClose(window)) {
    tfps->frame();

    if ( tfps->passedT > 1) {
      std::cout << "FPS: " << 1.0/tfps->dt << std::endl;
      tfps->resetTimer();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    standard->bind();
    standard->setUniform3f("u_Color", (1.0f+(float)sin(alpha))/2.0f, 1.0f, 1.0f);
    circlePoint->bind();
    circlePoint->setUniform3f("u_Color", (1.0f+(float)sin(alpha))/2.0f, 1.0f, 1.0f);

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 M3D = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix * scalingMatrix;
    standard->bind();
    standard->setUniformMat4fv("m_3d", M3D);
    circlePoint->bind();
    circlePoint->setUniformMat4fv("m_3d", M3D);
    glm::mat4 rot4D = rotYW(angle);

    standard->bind();
    standard->setUniformMat4fv("m_rot4d", rot4D);
    circlePoint->bind();
    circlePoint->setUniformMat4fv("m_rot4d", rot4D);

    standard->bind();
    glLineWidth(2.0f);
    cube->draw(GL_LINES, 32 * 2, 0);
    
    circlePoint->bind();
    glPointSize(10.0f);
    cube->draw(GL_POINTS, 16, 32 * 2);

    angle += glm::radians(45.0 * tfps->dt);
    alpha += 0.5 * tfps->dt;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete cube;
  delete standard;

  glfwTerminate();
  return 0;
}