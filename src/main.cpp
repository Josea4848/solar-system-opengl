#include "../include/camera.h"
#include "../include/solar_system.h"
#include "../include/sphere.h"
#include <GL/freeglut_std.h>
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

std::vector<Planet> planets;
Sphere *sun, *skybox;
SDL_Window *window = nullptr;
SDL_GLContext glContext;
Camera *camera;
bool pause = false;

void init(void) {
  // Luz definida em coordenadas de câmera (position.w = 0 para luz direcional)
  GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light_ambient[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

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
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Posiciona câmera
  glm::vec3 look_at = camera->getPosition() + camera->getFrontVector();
  glm::vec3 cam_pos = camera->getPosition();
  glm::vec3 v_up = camera->getVup();
  gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z, look_at.x, look_at.y, look_at.z,
            v_up.x, v_up.y, v_up.z);

  // Desenha sol
  skybox->draw();

  // Redefinir a posição da luz para coordenadas de câmera
  GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Desenha sol
  sun->draw();

  // Desenha cada planeta
  for (Planet &planet : planets) {
    glPushMatrix();
    Position pos = planet.model.getPosition();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(planet.rotate, 0.0f, 1.0f, 0.0f);
    planet.model.draw();
    glPopMatrix();

    // Atualiza translação do planeta para prox iteração
    if (!pause) {
      updatePlanet(planet);
      updateRotate(planet);
      planet.translation_value += 1.0;
    }
  }
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 2000.0);
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
      float x_offset = event.motion.xrel * camera->getSensitivity();
      float y_offset = -event.motion.yrel * camera->getSensitivity();

      // Atualiza inclinação da câmera (pitch e yaw)
      camera->updateAngles(x_offset, y_offset);
      break;
    }

    // Eventos do teclado
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_p)
        pause = !pause;
      else if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      break;
    }
  }
}

void moveCamera() {
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  float velocity = camera->getSpeed();
  glm::vec3 cam_pos = camera->getPosition();

  if (keystate[SDL_SCANCODE_LSHIFT]) {
    // Câmera em baixa velocidade
    camera->setSpeed(0.025f);
  } else {
    // Câmera em velocidade normal
    camera->setSpeed(0.1f);
  }
  if (keystate[SDL_SCANCODE_W])
    camera->setPosition(cam_pos + camera->getFrontVector() * velocity);
  if (keystate[SDL_SCANCODE_S])
    camera->setPosition(cam_pos - camera->getFrontVector() * velocity);
  if (keystate[SDL_SCANCODE_A])
    camera->setPosition(cam_pos - camera->getRightVector() * velocity);
  if (keystate[SDL_SCANCODE_D])
    camera->setPosition(cam_pos + camera->getRightVector() * velocity);
  if (keystate[SDL_SCANCODE_SPACE])
    camera->setPosition(cam_pos + camera->getWorldUp() * velocity);
  if (keystate[SDL_SCANCODE_LCTRL])
    camera->setPosition(cam_pos - camera->getWorldUp() * velocity);
}

void loadPlanets() {
  // Carregando skybox
  skybox =
      new Sphere(200.0, {0.0, 0.0, 0.0}, "../assets/models/skybox/skybox.jpeg");

  // Definindo sol
  GLfloat emission[] = {1.0f, 1.0f, 1.0f, 1.0f};
  sun = new Sphere(3.0, {-4.0, 0.0, 0.0}, "../assets/models/sol/2k_sun.jpeg",
                   emission);

  // Definindo planetas
  planets.push_back({Sphere(0.029, {4.0, 0.0, 0.0},
                            "../assets/models/mercurio/2k_mercury.jpeg"),
                     4.0, false, 0.027f, 0.0, 0});
  planets.push_back({Sphere(0.07, {6.0, 0.0, 0.0},
                            "../assets/models/venus/2k_venus_atmosphere.jpeg"),
                     6.0, false, 0.020f, 0.0, 0});
  planets.push_back({Sphere(0.075, {8.0, 0.0, 0.0},
                            "../assets/models/terra/EarthComposited_2k.png"),
                     8.0, false, 0.016f, 0.0, 0});
  planets.push_back(
      {Sphere(0.04, {11.0, 0.0, 0.0}, "../assets/models/marte/2k_mars.jpeg"),
       11.0, false, 0.010f, 0});
  planets.push_back({Sphere(0.85, {15.0, 0.0, 0.0},
                            "../assets/models/jupiter/2k_jupiter.jpeg"),
                     15.0, false, 0.008f, 0.0, 0});
  planets.push_back(
      {Sphere(0.7, {22.0, 0.0, 0.0}, "../assets/models/saturno/2k_saturn.jpeg"),
       22.0, true, 0.005f, 0.0, 0});

  planets.push_back(
      {Sphere(0.3, {25.0, 0.0, 0.0}, "../assets/models/urano/2k_uranus.jpeg"),
       25.0, false, 0.0025f, 0.0, 0});
  planets.push_back(
      {Sphere(0.3, {28.0, 0.0, 0.0}, "../assets/models/netuno/2k_neptune.jpeg"),
       28.0, false, 0.001f, 0.0, 0});
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

  // Inicializando parâmetros da câmera
  camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                      0.1f, -90.0f, 0.0f);

  init();

  // Criando modelos dos planetas e importando as texturas
  loadPlanets();

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
