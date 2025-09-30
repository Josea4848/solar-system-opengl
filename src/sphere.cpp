#include "../include/sphere.h"
#include <GL/gl.h>

Sphere::Sphere(double radius, glm::vec3 pos, const char *texture_path)
    : pos(pos), radius(radius) {
  this->texture_id = loadTexture(texture_path);
  this->emission[0] = 0.0f;
  this->emission[1] = 0.0f;
  this->emission[2] = 0.0f;
  this->emission[3] = 1.0f;
  this->pitch = 0.0;
  this->yaw = 0.0;
  this->roll = 0.0;
}

Sphere::Sphere(double radius, glm::vec3 pos, float pitch, float yaw, float roll,
               const char *texture_path)
    : pos(pos), radius(radius), pitch(pitch), yaw(yaw), roll(roll) {
  this->texture_id = loadTexture(texture_path);
  this->emission[0] = 0.0f;
  this->emission[1] = 0.0f;
  this->emission[2] = 0.0f;
  this->emission[3] = 1.0f;
}

Sphere::Sphere(double radius, glm::vec3 pos, const char *texture_path,
               GLfloat emission[4])
    : pos(pos), radius(radius) {
  this->texture_id = loadTexture(texture_path);
  this->emission[0] = emission[0];
  this->emission[1] = emission[1];
  this->emission[2] = emission[2];
  this->emission[3] = emission[3];
  this->pitch = 0.0;
  this->yaw = 0.0;
  this->roll = 0.0;
}

double Sphere::getRadius() { return this->radius; }
glm::vec3 Sphere::getPosition() { return this->pos; }

void Sphere::setPos(glm::vec3 new_pos) { this->pos = new_pos; }

void Sphere::draw() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);
  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, GL_TRUE);
  gluQuadricNormals(quad, GLU_SMOOTH);
  glPushMatrix();
  glScalef(-1.0f, 1.0f, 1.0f);

  // Rotação, por conta da textura
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

  glRotatef(this->pitch, 1.0f, 0.0f, 0.0f); // pitch
  glRotatef(this->yaw, 0.0f, 0.0f, 1.0f);   // yaw
  glRotatef(this->roll, 0.0f, 1.0f, 0.0f);  // roll

  gluSphere(quad, radius, 50, 50);
  gluDeleteQuadric(quad);
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

Sphere::~Sphere() {}

void Sphere::rotate(float pitch, float yaw, float roll) {
  this->pitch += pitch;
  if (this->pitch > 360.0)
    this->pitch = 0.0;

  this->yaw += yaw;
  if (this->yaw > 360.0)
    this->yaw = 0.0;

  this->roll += roll;
  if (this->roll > 360.0)
    this->roll = 0.0;
}
