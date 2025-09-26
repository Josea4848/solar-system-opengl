#include "../include/sphere.h"
#include <utility>

Sphere::Sphere(double radius, Position pos, const char *texture_path) {
  this->radius = radius;
  this->pos = std::move(pos);
  this->texture_id = loadTexture(texture_path);
}

double Sphere::getRadius() { return this->radius; }
Position Sphere::getPosition() { return this->pos; }

Sphere::~Sphere() {}
