#ifndef SPHERE_H
#define SPHERE_H

#include "texture.h"
#include <GL/gl.h>

typedef struct Position {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Position;

class Sphere {
public:
  Sphere(double radius, Position pos, const char *texture_path);
  double getRadius();
  Position getPosition();

  ~Sphere();

private:
  double radius;
  Position pos;
  GLuint texture_id;
};

#endif
