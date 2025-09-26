#include "../include/sphere.h"
#include <utility>

Sphere::Sphere(double radius, Position pos, const char *texture_path) {
  this->radius = radius;
  this->pos = std::move(pos);
  this->texture_id = loadTexture(texture_path);
}

double Sphere::getRadius() { return this->radius; }
Position Sphere::getPosition() { return this->pos; }

void Sphere::draw() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, GL_TRUE);
  gluSphere(quad, radius, 50, 50);
  gluDeleteQuadric(quad);

  glDisable(GL_TEXTURE_2D);
}

Sphere::~Sphere() {}
