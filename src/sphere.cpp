#include "../include/sphere.h"
#include <GL/gl.h>
#include <utility>

Sphere::Sphere(double radius, Position pos, const char *texture_path) {
  this->radius = radius;
  this->pos = std::move(pos);
  this->texture_id = loadTexture(texture_path);
}

double Sphere::getRadius() { return this->radius; }
Position Sphere::getPosition() { return this->pos; }

void Sphere::setPos(Position new_pos) { this->pos = new_pos; }

void Sphere::draw() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, GL_TRUE);
  glPushMatrix();
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  gluSphere(quad, radius, 50, 50);
  gluDeleteQuadric(quad);
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

Sphere::~Sphere() {}
