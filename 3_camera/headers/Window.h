#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{

public:
  Window();

  Window(GLint windowWidth, GLint windowHeight);

  int Initialise();

  GLint getBufferWidth(){ return bufferWidth; }
  GLint getBufferHeight(){ return bufferHeight; }

  bool getShouldClose(){ return glfwWindowShouldClose(mainWindow);}
  void swapBuffers() {glfwSwapBuffers(mainWindow);}

  ~Window();

private:
  GLFWwindow *mainWindow;

  GLint width, height;
  GLint bufferWidth, bufferHeight;

  bool keys[1024];
  
  void createCallbacks();

  static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); 
};
