#include "../include/ring.h"

Ring::Ring(float inner_radius, float outer_radius, int segments,
           const char *texture_path)
    : inner_radius(inner_radius), outer_radius(outer_radius),
      segments(segments) {
  this->texture_id = loadTexture(texture_path);
  this->pitch = 0.0;
  this->yaw = 0.0;
  this->roll = 0.0;
}

Ring::Ring(float inner_radius, float outer_radius, int segments, float pitch,
           float yaw, float roll, const char *texture_path)
    : inner_radius(inner_radius), outer_radius(outer_radius),
      segments(segments), pitch(pitch), yaw(yaw), roll(roll) {
  this->texture_id = loadTexture(texture_path);
}

void Ring::draw() {
  glPushMatrix();
  glTranslatef(this->position.x, this->position.y, this->position.z);
  glRotatef(this->pitch, 1.0f, 0.0f, 0.0f); // pitch
  glRotatef(this->yaw, 0.0f, 1.0f, 0.0f);   // yaw
  glRotatef(this->roll, 0.0f, 0.0f, 1.0f);  // roll

  // Habilitar textura
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, this->texture_id);

  // Desabilitar iluminação para o anel para melhor visualização
  glDisable(GL_LIGHTING);

  glBegin(GL_QUAD_STRIP);

  GLfloat inner = this->inner_radius;
  GLfloat outer = this->outer_radius;

  for (int i = 0; i <= this->segments; i++) {
    GLfloat angle = 2.0f * M_PI * i / this->segments;
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

void Ring::setPosition(glm::vec3 new_pos) { this->position = new_pos; }

// Aplica rotação no modelo
void Ring::rotate(float pitch, float yaw, float roll) {
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
