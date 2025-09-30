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
#include <random>
#include <cmath>

// Câmera
typedef struct
{
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;  // Vetor up global de referência
  float     speed;
  float     yaw;
  float     pitch;
  float     sensitivity;
} Camera;

// Estrutura para anéis planetários
typedef struct Ring
{
  float inner_radius;
  float outer_radius;
  std::string texture_path;
} Ring;

typedef struct Planet
{
  Sphere  model;
  float   distance;
  bool    has_ring;
  Ring    ring;  // Anel do planeta
  GLfloat translation_rate;
  GLfloat translation_value;
  int     rotate;
} Planet;

void updatePlanet(Planet &planet) {
  static GLfloat t  = 0.0;
  double         R  = planet.distance;
  double         dt = planet.translation_value;
  t                 = dt * planet.translation_rate / 10;

  planet.model.setPos({(GLfloat)R * cos(t), (GLfloat)0.0, (GLfloat)R * sin(t)});
}

void updateRotate(Planet &planet) {
  planet.rotate += 1.0f;
  if (planet.rotate >= 360.0f) {
    planet.rotate -= 360.0f;
  }
}

std::vector<Planet> planets;
Sphere             *sun, *skybox;
SDL_Window         *window = nullptr;
SDL_GLContext       glContext;
Camera              camera;
bool                pause = false;

// Variáveis para textura do anel
GLuint ring_texture = 0;
void drawRing(const Planet& planet, const Position& planetPos) {
    if (!planet.has_ring) return;
    
    glPushMatrix();
    glTranslatef(planetPos.x, planetPos.y, planetPos.z);
    glRotatef(planet.rotate, 0.0f, 1.0f, 0.0f);
    
    // Habilitar textura
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ring_texture);
    
    // Desabilitar iluminação para o anel para melhor visualização
    glDisable(GL_LIGHTING);
    
    // Cor base para o anel (caso a textura não carregue)
    glColor3f(0.8f, 0.8f, 0.6f);
    
    // Desenhar o anel como um disco fino
    GLfloat inner = planet.ring.inner_radius;
    GLfloat outer = planet.ring.outer_radius;
    GLint slices = 50;
    GLint loops = 2;
    
    gluDisk(gluNewQuadric(), inner, outer, slices, loops);
    
    // Reabilitar iluminação
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

void drawRingWithQuads(const Planet& planet, const Position& planetPos) {
    if (!planet.has_ring) return;

    glPushMatrix();
    glTranslatef(planetPos.x, planetPos.y, planetPos.z);
    glRotatef(planet.rotate, 0.0f, 1.0f, 0.0f);
    
    // Habilitar textura
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ring_texture);
    
    // Desabilitar iluminação para o anel para melhor visualização
    glDisable(GL_LIGHTING);
    
    glBegin(GL_QUAD_STRIP);
    
    GLfloat inner = planet.ring.inner_radius;
    GLfloat outer = planet.ring.outer_radius;
    int segments = 100;
    
    for (int i = 0; i <= segments; i++) {
        GLfloat angle = 2.0f * M_PI * i / segments;
        GLfloat cosA = cos(angle);
        GLfloat sinA = sin(angle);
        
        // Ponto interno
        glTexCoord2f(0.0f, (GLfloat)i / segments);
        glVertex3f(inner * cosA, 0.0f, inner * sinA);
        
        // Ponto externo
        glTexCoord2f(1.0f, (GLfloat)i / segments);
        glVertex3f(outer * cosA, 0.0f, outer * sinA);
    }
    
    glEnd();
    
    // Reabilitar iluminação
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

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
  GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light_ambient[]  = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  // Configurar a luz
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // Carregar textura do anel
  ring_texture = loadTexture("../assets/models/saturno/2k_saturn_ring_alpha.png");
  // Se não carregar, use uma textura alternativa ou desenhe sem textura

  // Configurar captura do mouse
  SDL_SetRelativeMouseMode(SDL_TRUE);

  // Calcular vetores iniciais
  updateCameraVectors();
}

void drawSun() {
  // Desenha sol
  GLfloat emission[] = {1.0f, 1.0f, 0.5f, 1.0f};
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);
  sun->draw();
  GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glm::vec3 center = camera.position + camera.front;
  gluLookAt(camera.position.x, camera.position.y, camera.position.z, center.x, center.y, center.z, camera.up.x, camera.up.y, camera.up.z);

  skybox->draw();

  // Redefinir a posição da luz para coordenadas de câmera
  GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Desenha sol
  drawSun();

  // Desenha cada planeta
  for (Planet &planet : planets) {
    // Obter posição do planeta (não constante)
    Position planetPos = planet.model.getPosition();
    
    glPushMatrix();
    glTranslatef(planetPos.x, planetPos.y, planetPos.z);
    glRotatef(planet.rotate, 0.0f, 1.0f, 0.0f);
    planet.model.draw();
    glPopMatrix();

    // Desenha o anel se o planeta tiver
    if (planet.has_ring) {
        drawRingWithQuads(planet, planetPos);
    }

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
        float xoffset = event.motion.xrel * camera.sensitivity;
        float yoffset = -event.motion.yrel * camera.sensitivity;

        camera.yaw += xoffset;
        camera.pitch += yoffset;

        // Limitar pitch para evitar flip
        if (camera.pitch > 89.0f)
          camera.pitch = 89.0f;
        if (camera.pitch < -89.0f)
          camera.pitch = -89.0f;

        updateCameraVectors();
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
  float        velocity = camera.speed;

  if (keystate[SDL_SCANCODE_LSHIFT])
    camera.speed = 0.025f;  // velocidade padrão da câmera
  else
    camera.speed = 0.1f;

  if (keystate[SDL_SCANCODE_W])
    camera.position += camera.front * velocity;
  if (keystate[SDL_SCANCODE_S])
    camera.position -= camera.front * velocity;
  if (keystate[SDL_SCANCODE_A])
    camera.position -= camera.right * velocity;
  if (keystate[SDL_SCANCODE_D])
    camera.position += camera.right * velocity;
  if (keystate[SDL_SCANCODE_SPACE])
    camera.position += camera.worldUp * velocity;
  if (keystate[SDL_SCANCODE_LCTRL])
    camera.position -= camera.worldUp * velocity;
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL Error:" << SDL_GetError() << std::endl;
    return -1;
  }

  // Configurações OpenGL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow("Solar system", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) {
    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
  }

  // Configuração inicial da câmera
  camera.position    = glm::vec3(0.0f, 0.0f, 20.0f);
  camera.worldUp     = glm::vec3(0.0f, 1.0f, 0.0f);
  camera.sensitivity = 0.1f;
  camera.yaw         = -90.0f;
  camera.pitch       = 0.0f;

  // Calcular vetores iniciais
  updateCameraVectors();

  init();

  // Carregando skybox
  skybox = new Sphere(200.0, {0.0, 0.0, 0.0}, "../assets/models/skybox/skybox.jpeg");

  // Definindo sol
  sun = new Sphere(3.0, {-4.0, 0.0, 0.0}, "../assets/models/sol/2k_sun.jpeg");

  // Definindo planetas
  planets.push_back({Sphere(0.029, {4.0, 0.0, 0.0}, "../assets/models/mercurio/2k_mercury.jpeg"), 4.0, false, {0, 0, ""}, 0.027f, 0.0, 0});
  planets.push_back({Sphere(0.07, {6.0, 0.0, 0.0}, "../assets/models/venus/2k_venus_atmosphere.jpeg"), 6.0, false, {0, 0, ""}, 0.020f, 0.0, 0});
  planets.push_back({Sphere(0.075, {8.0, 0.0, 0.0}, "../assets/models/terra/EarthComposited_2k.png"), 8.0, false, {0, 0, ""}, 0.016f, 0.0, 0});
  planets.push_back({Sphere(0.04, {11.0, 0.0, 0.0}, "../assets/models/marte/2k_mars.jpeg"), 11.0, false, {0, 0, ""}, 0.010f, 0.0, 0});
  planets.push_back({Sphere(0.85, {15.0, 0.0, 0.0}, "../assets/models/jupiter/2k_jupiter.jpeg"), 15.0, false, {0, 0, ""}, 0.008f, 0.0, 0});
  
  // Saturno com anel
  planets.push_back({
    Sphere(0.7, {22.0, 0.0, 0.0}, "../assets/models/saturno/2k_saturn.jpeg"), 
    22.0, 
    true, 
    {1.0, 1.3, "../assets/models/saturno/2k_saturn.jpeg"},  // inner_radius, outer_radius, texture
    0.005f, 
    0.0, 
    0
  });
  
  planets.push_back({Sphere(0.3, {25.0, 0.0, 0.0}, "../assets/models/urano/2k_uranus.jpeg"), 25.0, false, {0, 0, ""}, 0.0025f, 0.0, 0});
  planets.push_back({Sphere(0.3, {28.0, 0.0, 0.0}, "../assets/models/netuno/2k_neptune.jpeg"), 28.0, false, {0, 0, ""}, 0.001f, 0.0, 0});

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
  if (ring_texture != 0) {
      glDeleteTextures(1, &ring_texture);
  }
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
