#include <iostream>
#include <string.h>
#include <cmath>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex shader
static const char* vShader = "     \n\
#version 330                       \n\
layout (location=0) in vec3 pos;   \n\
out vec4 vCol;                     \n\
                                    \n\
uniform mat4 model;                 \n\
void main(){                       \n\
  gl_Position = model * vec4(pos, 1.0);\n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n\
} " ; 


// Fragment shader
static const char* fShader =  "     \n\
#version 330                       \n\
in vec4 vCol;                      \n\
out vec4 colour;                   \n\
                                    \n\
void main(){                       \n\
  colour = vCol; \n\
}"; 


void CreateTriangle(){

  unsigned int indices[]={
    0,3,1,
    1,3,2,
    2,3,0,
    0,1,2

  };

  GLfloat vertices[]={
    -1.0f, -1.0f, 0.0f,
     0.0f, -1.0f, 1.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f
  };

  glGenVertexArrays(1, &VAO); //creating VAO
  glBindVertexArray(VAO); //bind VAO

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO); //creating VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){

  GLuint theShader = glCreateShader(shaderType);

  const GLchar* theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(theShader, 1, theCode, codeLength);
  glCompileShader(theShader);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); //Program

  if(!result){
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling %d shader: '%s'\n",shaderType, eLog);
    return;
  }

  glAttachShader(theProgram, theShader);

}


void CompileShaders(){
  shader = glCreateProgram();

  if(!shader){
    printf("Error creating shader program!\n");
    return;
  }
  
  AddShader(shader, vShader, GL_VERTEX_SHADER);
  AddShader(shader, fShader, GL_FRAGMENT_SHADER);
  
  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);

  if(!result){
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }
 
  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

  if(!result){
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }

  uniformModel = glGetUniformLocation(shader, "model");

}

int main(){
  
  if(!glfwInit()){
   printf("GLFW initialization failed!");
   glfwTerminate();
   return 1;
  }

  // Setup GLFW window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile = No backwards 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
  if(!mainWindow){
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set the context for GLEW to use

  glfwMakeContextCurrent(mainWindow);

  // Allow modern extention features
  glewExperimental = GL_TRUE;

  if(glewInit() != GLEW_OK){
    printf("GLEW initialization failed!");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }


  glEnable(GL_DEPTH_TEST);


  // Setup viewport size

  glViewport(0, 0, bufferWidth, bufferHeight);

  CreateTriangle();
  CompileShaders();


  // Loop until window closed

  while(!glfwWindowShouldClose(mainWindow)){
    // get and handle user input events
    glfwPollEvents();

    if(direction){
      triOffset += triIncrement;
    }else{
      triOffset -= triIncrement;
    }

    if(abs(triOffset) >= triMaxOffset){
      direction = !direction;
    }
    
    curAngle += 1.0f;

    if(curAngle >= 360){
      curAngle -= 360;
    }

    if(sizeDirection){
      curSize += 0.001f;
    }else{
      curSize -= 0.001f;
    }
    if(curSize >= maxSize || curSize <= minSize){
      sizeDirection = !sizeDirection;
    }


    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glUseProgram(shader);

    glm::mat4 model = glm::mat4(1.0);  
    //std::cout << glm::to_string(model) << std::endl;
    //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
    model = glm::rotate(model, curAngle*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
  }

  return 0;
}
