#include "../include/solar_system.h"
#include <glm/geometric.hpp>

void updatePlanet(Planet &planet) {
  static GLfloat t = 0.0;
  double R = planet.distance;
  double dt = planet.translation_value;
  t = dt * planet.translation_rate / 10;

  planet.model.setPos(
      {(GLfloat)(R * cos(t)), (GLfloat)0.0f, (GLfloat)(R * sin(t))});
}

void updateRotate(Planet &planet) {
  planet.rotate += 1.0f;
  if (planet.rotate >= 360.0f) {
    planet.rotate -= 360.0f;
  }
}
