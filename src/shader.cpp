#include "../include/shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {

  // Recupera o código fonte do vertex/fragment do filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Assegura que objetos ifstream podem lançar exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // abre arquivos
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    // Lê conteúdo do buffer dos files
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // fecha files
    vShaderFile.close();
    fShaderFile.close();

    // Converte streams em string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // Compila shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // Printa erros de compilação, se existir
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  // Printa erros de compilação, se existir
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Programa do shader
  this->id = glCreateProgram();
  glAttachShader(this->id, vertex);
  glAttachShader(this->id, fragment);
  glLinkProgram(this->id);

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->id, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Deleta shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(this->id); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}
