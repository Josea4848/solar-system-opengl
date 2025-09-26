#include "../include/sphere.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

std::vector<Sphere> planets;
void init(void) {
  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat mat_shininess[] = {200.0};

  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  for (Sphere planet : planets) {
    Position pos = planet.getPosition();

    glPushMatrix();
    planet.draw();
    glPopMatrix();
  }
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void handleEvents(bool &running) {
  SDL_Event event;

  // Processa eventos
  while (SDL_PollEvent(&event)) {
    // Eventos de janela
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        reshape(event.window.data1, event.window.data2);
        break;

      case SDL_WINDOWEVENT_CLOSE:
        running = false;
      }
    }
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
    }
  }
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL Error:" << SDL_GetError()
              << std::endl;
    return -1;
  }

  // Indica versão do opengl
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_Window *window = SDL_CreateWindow(
      "Solar system", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280,
      720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return -1;
  }

  // Cria contexto opengl
  SDL_GLContext glContext = SDL_GL_CreateContext(window);

  if (!glContext) {
    std::cerr << "OpenGL context could not be created! SDL Error: "
              << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  // Loop principal do fluxo de execução
  bool running = true;

  planets.push_back(Sphere(0.5, {0.0, 0.0, 0.0},
                           "../assets/models/terra/EarthComposited_2k.png"));

  // Configura viewport inicial
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  reshape(w, h);

  // Loop principal
  while (running) {
    handleEvents(running);
    display();
    SDL_GL_SwapWindow(window);
    // SDL_Delay(16); // ~60 FPS
  }

  // Limpeza
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
