#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

#include "shaders.h"

GLFWwindow *window = nullptr;
GLuint shaderProgram = 0;
GLint uTimeLoc = -1;
GLint uResLoc = -1;

// func for compilating 1 shader
GLuint compileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "[OpenGL error] shader compilation fail:\n"
              << infoLog << std::endl;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

// Link program (func for shader program comp)
GLuint createShaderProgram(const char *vsSource, const char *fsSource) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cerr << "[OpenGL Error] Program Linking Failed:\n"
              << infoLog << std::endl;
    return 0;
  }

  // after linkink we can delete shader
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void renderFrame() {
  int width, height;
  emscripten_get_canvas_element_size("#bg-canvas", &width, &height);

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);

  // time
  float currentTime = static_cast<float>(emscripten_get_now()) / 1000.0f;
  glUniform1f(uTimeLoc, currentTime);

  // res
  glUniform2f(uResLoc, static_cast<float>(width), static_cast<float>(height));

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glfwSwapBuffers(window);
}

int main() {
  if (!glfwInit()) {
    std::cerr << "No glfw, error" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(800, 600, "BG-engine", nullptr, nullptr);
  if (!window) {
    std::cerr << "No window creation, error" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  shaderProgram = createShaderProgram(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);

  uTimeLoc = glGetUniformLocation(shaderProgram, "u_time");
  uResLoc = glGetUniformLocation(shaderProgram, "u_resolution");

  float vertices[] = {
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
  };

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "a_position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

  emscripten_set_main_loop(renderFrame, 0, 1);

  return 0;
}
