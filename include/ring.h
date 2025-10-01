#ifndef RING_H
#define RING_H

#include "texture.h"
#include <GL/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

class Ring {
private:
  float inner_radius;
  float outer_radius;
  int segments;
  float pitch;
  float yaw;
  float roll;
  GLuint texture_id;
  glm::vec3 position;

public:
  Ring(float inner_radius, float outer_radius, int segments,
       const char *texture_path);
  Ring(float inner_radius, float outer_radius, int segments, float pitch,
       float yam, float roll, const char *texture_path);

  void setPosition(glm::vec3 new_pos);
  void rotate(float pitch, float yaw, float roll);
  void draw();
};

#endif
