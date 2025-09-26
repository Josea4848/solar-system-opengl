#include "../include/sphere.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>

std::vector<Sphere> planets;
SDL_Window *window = nullptr;
SDL_GLContext glContext;

// Câmera
typedef struct {
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp; // Vetor up global de referência
  float speed;
  float yaw;
  float pitch;
  float sensitivity;
} Camera;

Camera camera;

// Função completa para atualizar TODOS os vetores da câmera
void updateCameraVectors() {
  // Calcular vetor frontal baseado em yaw e pitch
  glm::vec3 front;
  front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
  front.y = sin(glm::radians(camera.pitch));
  front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

  camera.front = glm::normalize(front);

  // Calcular vetor direito usando worldUp como referência
  camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));

  // Calcular vetor up real
  camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}

void init(void) {
  // Luz definida em coordenadas de câmera (position.w = 0 para luz direcional)
  GLfloat light_position[] = {0.0, 0.0, 1.0,
                              0.0}; // Direção da luz (em relação à câmera)
  GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  // Configurar a luz
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // Configurar captura do mouse
  SDL_SetRelativeMouseMode(SDL_TRUE);

  // Calcular vetores iniciais
  updateCameraVectors();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glm::vec3 center = camera.position + camera.front;
  gluLookAt(camera.position.x, camera.position.y, camera.position.z, center.x,
            center.y, center.z, camera.up.x, camera.up.y, camera.up.z);

  // Redefinir a posição da luz para coordenadas de câmera
  GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  GLfloat mat_shininess[] = {200.0}; // Aumentado para melhor reflexão
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  for (Sphere planet : planets) {
    glPushMatrix();
    planet.draw();
    glPopMatrix();
  }
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1,
                 200.0); // Campo de visão mais amplo
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void handleEvents(bool &running) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;

    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        reshape(event.window.data1, event.window.data2);
        break;
      case SDL_WINDOWEVENT_CLOSE:
        running = false;
        break;
      }
      break;

    case SDL_MOUSEMOTION: {
      // CORREÇÃO: Colocar o bloco do mouse motion entre chaves
      float xoffset = event.motion.xrel * camera.sensitivity;
      float yoffset = -event.motion.yrel * camera.sensitivity;

      camera.yaw += xoffset;
      camera.pitch += yoffset;

      // Limitar pitch para evitar flip
      if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
      if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

      // ATUALIZAÇÃO CORRIGIDA: Chamar apenas updateCameraVectors()
      updateCameraVectors();
      break;
    }

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      break;
    }
  }
}

void moveCamera() {
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  float velocity = camera.speed;

  if (keystate[SDL_SCANCODE_W])
    camera.position += camera.front * velocity;
  if (keystate[SDL_SCANCODE_S])
    camera.position -= camera.front * velocity;
  if (keystate[SDL_SCANCODE_A])
    camera.position -= camera.right * velocity;
  if (keystate[SDL_SCANCODE_D])
    camera.position += camera.right * velocity;
  if (keystate[SDL_SCANCODE_SPACE])
    camera.position +=
        camera.worldUp *
        velocity; // Usar worldUp para movimento vertical consistente
  if (keystate[SDL_SCANCODE_LCTRL])
    camera.position -= camera.worldUp * velocity;

  // ATUALIZAÇÃO OPCIONAL: Só é necessário se o movimento alterar a orientação
  // updateCameraVectors(); // Removido - não é necessário após movimento puro
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL Error:" << SDL_GetError()
              << std::endl;
    return -1;
  }

  // Configurações OpenGL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow("Solar system", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 1280, 720,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return -1;
  }

  glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    std::cerr << "OpenGL context could not be created! SDL Error: "
              << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) {
    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError()
              << std::endl;
  }

  // Configuração inicial da câmera
  camera.position = glm::vec3(0.0f, 0.0f, 20.0f);
  camera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // Vetor up global de referência
  camera.speed = 0.1f;
  camera.sensitivity = 0.1f;
  camera.yaw = -90.0f;
  camera.pitch = 0.0f;

  // Calcular vetores iniciais
  updateCameraVectors();

  init();

  planets.push_back(Sphere(0.5, {0.0, 0.0, 0.0},
                           "../assets/models/terra/EarthComposited_2k.png"));

  // Configura viewport inicial
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  reshape(w, h);

  // Loop principal
  bool running = true;
  while (running) {
    handleEvents(running);
    moveCamera();
    display();
    SDL_GL_SwapWindow(window);
  }

  // Limpeza
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
