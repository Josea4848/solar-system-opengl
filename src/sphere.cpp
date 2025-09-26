#include "../include/sphere.h"
#include <utility>

Sphere::Sphere(double radius, Position pos, double rotate, double rotate_rate) {
  this->radius = radius;
  this->pos = std::move(pos);
  this->rotate = rotate;
  this->rotate_rate = rotate_rate;
}
double Sphere::getRadius() { return this->radius; }
Position Sphere::getPosition() { return this->pos; }

Sphere::~Sphere() {}
