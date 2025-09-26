cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(solar)

# Definir padrão C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adicionar diretório de includes
include_directories(include)

# Definir arquivos fonte
set(SOURCES
    src/main.cpp
    src/texture.cpp
    src/sphere.cpp
)

# Criar executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Linkar bibliotecas exatamente como no comando g++ original
target_link_libraries(${PROJECT_NAME} 
    GL
    GLU
    glut
    m
)

# Copiar assets para o diretório de build
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Mensagem informativa
message(STATUS "Projeto: ${PROJECT_NAME} configurado com sucesso")
