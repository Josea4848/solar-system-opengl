#ifndef SPHERE_H
#define SPHERE_H

#include "texture.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <array>
#include <glm/glm.hpp>

class Sphere {
public:
  Sphere(double radius, glm::vec3 pos, const char *texture_path);
  Sphere(double radius, glm::vec3 pos, float pitch, float yaw, float roll,
         const char *texture_path);
  Sphere(double radius, glm::vec3 pos, const char *texture_path,
         GLfloat emission[4]);
  double getRadius();
  glm::vec3 getPosition();

  void draw();
  void setPos(glm::vec3 new_pos);

  void rotate(float pitch, float yaw, float roll);

  ~Sphere();

private:
  double radius;
  glm::vec3 pos;
  GLuint texture_id;
  GLfloat emission[4];
  float pitch;
  float yaw;
  float roll;
};

#endif
