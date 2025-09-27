cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(solar)

# Definir padrão C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definir política para OpenGL
cmake_policy(SET CMP0072 NEW)

# Adicionar diretório de includes
include_directories(include)

# Encontrar pacotes necessários
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PkgConfig REQUIRED)

# Encontrar SDL2
pkg_check_modules(SDL2 REQUIRED sdl2)

# Para sistemas que usam freeglut
if(PKG_CONFIG_FOUND)
    pkg_check_modules(FREEGLUT freeglut)
endif()

# Verificar se os pacotes foram encontrados
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL não encontrado!")
endif()

if(NOT GLUT_FOUND)
    message(FATAL_ERROR "GLUT não encontrado!")
endif()

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 não encontrado!")
endif()

# Definir arquivos fonte
set(SOURCES
    src/main.cpp
    src/texture.cpp
    src/sphere.cpp
)

# Definir arquivos header
set(HEADERS
    include/texture.h
    include/sphere.h
    include/stb_image.h
)

# Criar executável
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Adicionar include directories do SDL2
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARY}
    ${SDL2_LIBRARIES}
    m  # biblioteca matemática
)

# Adicionar flags de compilação do SDL2
target_compile_options(${PROJECT_NAME} PRIVATE ${SDL2_CFLAGS_OTHER})

# Compilador específico flags (opcional)
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Definir diretório de saída
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Copiar assets para o diretório de build (opcional)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Mensagens informativas
message(STATUS "Projeto: ${PROJECT_NAME}")
message(STATUS "OpenGL encontrado: ${OPENGL_FOUND}")
message(STATUS "GLUT encontrado: ${GLUT_FOUND}")
message(STATUS "SDL2 encontrado: ${SDL2_FOUND}")
message(STATUS "Diretório de build: ${CMAKE_BINARY_DIR}")
