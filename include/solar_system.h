#include "ring.h"
#include "sphere.h"

typedef struct Planet {
  Sphere *model;
  Ring *ring;
  GLfloat distance;
  GLfloat translation_rate;
  GLfloat translation_value;
  float rotate_rate;
} Planet;

void updatePlanet(Planet &planet);
