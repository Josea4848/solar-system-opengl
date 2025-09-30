#include "../include/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float sensitivity,
               float yam, float pitch) {
  // Configuração inicial da câmera
  this->position = glm::vec3(0.0f, 0.0f, 20.0f);
  this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->sensitivity = 0.1f;
  this->yaw = -90.0f;
  this->pitch = 0.0f;

  // Atualização interna de vetores
  updateVectors();
}

void Camera::updateAngles(double x_offset, double y_offset) {
  this->yaw += x_offset;
  this->pitch += y_offset;

  // Limitar pitch para evitar flip
  if (this->pitch > 89.0f)
    this->pitch = 89.0f;
  if (this->pitch < -89.0f)
    this->pitch = -89.0f;

  updateVectors();
}

void Camera::updateVectors() {
  // Calcular vetor frontal baseado em yaw e pitch
  glm::vec3 front;
  front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  front.y = sin(glm::radians(this->pitch));
  front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->front = glm::normalize(front);

  // Calcular vetor direito usando worldUp como referência
  this->right = glm::normalize(glm::cross(this->front, this->worldUp));

  // Calcular vetor up real
  this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::setSpeed(float speed) { this->speed = speed; }

void Camera::setPosition(glm::vec3 position) { this->position = position; }

float Camera::getSensitivity() { return this->sensitivity; }

float Camera::getSpeed() { return this->speed; }

glm::vec3 Camera::getFrontVector() { return this->front; }

glm::vec3 Camera::getRightVector() { return this->right; }

glm::vec3 Camera::getWorldUp() { return this->worldUp; }

glm::vec3 Camera::getPosition() { return this->position; }

glm::vec3 Camera::getVup() { return this->up; }
