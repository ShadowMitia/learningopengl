#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <cmath>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

  glViewport(0, 0, 800, 600);


  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



  // vertex shaders are used to normalise coordinates to openGL's visible region
  auto vertexShaderSource {
			   "#version 330 core\n"	\
			   "layout (location = 0) in vec3 aPos;\n"	\
			   "layout (location = 1) in vec3 aColor;\n"    \
			   "out vec3 ourColor;\n" \
			   "out vec4 position;\n" \
			   "uniform vec3 offset;\n" \
			   "void main()\n"				\
			   "{\n"					\
			   "    position = vec4(aPos.x + offset.x, -1 * aPos.y, aPos.z, 1.0);\n" \
			   "gl_Position = position\n;" \
			   "   ourColor = aColor;\n"			\
			   "}\n"					\
  };

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
  }

  // fragment shaders are about calculating color output

  auto fragmentShaderSource{
    "#version 330 core\n"			        \
      "out vec4 FragColor;\n"			         \
      "in vec3 ourColor;\n" \
      "in vec4 position;\n"\
      "void main()\n"				        \
      "{\n"						\
      "FragColor = vec4(position);\n"	\
      "} \n"						\
      };


  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
  }

  auto fragmentShaderOwnColorSource{
    "#version 330 core\n"                               \
      "uniform vec4 ourColor;\n"				\
      "out vec4 FragColor;\n"			        \
      "void main()\n"				        \
      "{\n"						\
      "FragColor = vec4(ourColor);\n"	\
      "} \n"						\
      };


  unsigned int fragmentOwnColorShader;
  fragmentOwnColorShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentOwnColorShader, 1, &fragmentShaderOwnColorSource, nullptr);
  glCompileShader(fragmentOwnColorShader);

  glGetShaderiv(fragmentOwnColorShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentOwnColorShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENTOWNCOLOR::COMPILATION_FAILED\n" << infoLog << '\n';
  }

  // compuler shader program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
  }

  unsigned int shaderProgramOwnColor = glCreateProgram();
  glAttachShader(shaderProgramOwnColor, vertexShader);
  glAttachShader(shaderProgramOwnColor, fragmentOwnColorShader);
  glLinkProgram(shaderProgramOwnColor);

  glGetProgramiv(shaderProgramOwnColor, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgramOwnColor, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADEROWNCOLOR::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(fragmentOwnColorShader);

  //

  float triangle_vertices_1[] = {
				 -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
				 0.5f, -0.5f, 0.0f,  0.f, 1.f, 0.f,
				 0.0f,  0.5f, 0.0f,  0.f, 0.f, 1.f
  };

  float triangle_vertices_2[] = {
				 -0.8f, 0.9f, 0.0f,
				 -0.6f, 0.9f, 0.0f,
				 -0.7f, 0.5f, 0.0f
  };


  // VAO
  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  glBindVertexArray(VAO[0]);

  // VBOs are used to send large batches of vertices to the GPU
  unsigned int VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices_1), triangle_vertices_1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),  reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(VAO[1]);

  unsigned int VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices_2), triangle_vertices_2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
  glEnableVertexAttribArray(0);


  while(!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      float timeValue = static_cast<float>(glfwGetTime());
      int vertexOffsetLocation = glGetUniformLocation(shaderProgram, "offset");
      glUniform3f(vertexOffsetLocation, timeValue, 0, 0);

      glUseProgram(shaderProgram);

      glBindVertexArray(VAO[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glUseProgram(shaderProgramOwnColor);

      // update the uniform color

      float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
      int vertexColorLocation = glGetUniformLocation(shaderProgramOwnColor, "ourColor");
      glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

      glBindVertexArray(VAO[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);


      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(1, &VBO1);
  glDeleteBuffers(1, &VBO2);

  glfwTerminate();

  return 0;
}
