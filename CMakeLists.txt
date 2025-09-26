
cmake_minimum_required(VERSION 3.16)
project(SistemaSolar)

# Define o padrão C++ (C++17 é suficiente)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona warnings (opcional, mas recomendado)
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

# Adiciona o diretório include
include_directories(${PROJECT_SOURCE_DIR}/include)

# Lista todos os arquivos fonte
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/esfera/*.cpp
)

# Adiciona o executável
add_executable(SistemaSolar ${SOURCES})

# Procurando OpenGL
find_package(OpenGL REQUIRED)

# Procurando GLUT
find_package(GLUT REQUIRED)

# Adiciona as bibliotecas necessárias
target_link_libraries(SistemaSolar
    OpenGL::GL
    GLUT::GLUT
)

# Se precisar de assets (como modelos, texturas), você pode copiar para a pasta build
add_custom_command(TARGET SistemaSolar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SistemaSolar>/assets
)
