#ifndef CAMERA_H
#define CAMERA_H

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

class Camera {
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  float speed;
  float yaw;
  float pitch;
  float sensitivity;

  // Métodos de atualização dos vetores da câmera
  void updateVectors();

public:
  Camera(glm::vec3 position, glm::vec3 worldUp, float sensitivity, float yam,
         float pitch);
  void updateAngles(double x_offset, double y_offset);
  void setSpeed(float speed);
  void setPosition(glm::vec3 position);

  float getSensitivity();
  float getSpeed();
  glm::vec3 getFrontVector();
  glm::vec3 getRightVector();
  glm::vec3 getWorldUp();
  glm::vec3 getPosition();
  glm::vec3 getVup();
};

#endif
