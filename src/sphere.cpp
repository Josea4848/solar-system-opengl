#include "../include/sphere.h"

Sphere::Sphere(double radius, Position pos, const char *texture_path)
    : pos(pos), radius(radius) {
  this->texture_id = loadTexture(texture_path);
  this->emission[0] = 0.0f;
  this->emission[1] = 0.0f;
  this->emission[2] = 0.0f;
  this->emission[3] = 1.0f;
}

Sphere::Sphere(double radius, Position pos, const char *texture_path,
               GLfloat emission[4])
    : pos(pos), radius(radius) {
  this->texture_id = loadTexture(texture_path);
  this->emission[0] = emission[0];
  this->emission[1] = emission[1];
  this->emission[2] = emission[2];
  this->emission[3] = emission[3];
}

double Sphere::getRadius() { return this->radius; }
Position Sphere::getPosition() { return this->pos; }

void Sphere::setPos(Position new_pos) { this->pos = new_pos; }

void Sphere::draw() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);
  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, GL_TRUE);
  gluQuadricNormals(quad, GLU_SMOOTH);
  glPushMatrix();
  glScalef(-1.0f, 1.0f, 1.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  gluSphere(quad, radius, 50, 50);
  gluDeleteQuadric(quad);
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

Sphere::~Sphere() {}
