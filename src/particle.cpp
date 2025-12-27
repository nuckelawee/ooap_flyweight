#include "particle.h"
#include <algorithm>

Particle::Particle(float x, float y, float vx, float vy,
                   std::shared_ptr<ParticleType> type)
    : x_(x), y_(y), vx_(vx), vy_(vy), type_(type) {
}

void Particle::Update(float delta_time, float width, float height) {
  // Apply gravity
  vy_ += kGravity * delta_time;

  // Update position
  x_ += vx_ * delta_time;
  y_ += vy_ * delta_time;

  // Apply damping
  vx_ *= kDamping;
  vy_ *= kDamping;

  // Collision with walls
  float radius = type_->radius();
  if (x_ - radius < 0) {
    x_ = radius;
    vx_ = -vx_ * 0.8f;  // Energy loss on bounce
  } else if (x_ + radius > width) {
    x_ = width - radius;
    vx_ = -vx_ * 0.8f;
  }

  if (y_ - radius < 0) {
    y_ = radius;
    vy_ = -vy_ * 0.8f;
  } else if (y_ + radius > height) {
    y_ = height - radius;
    vy_ = -vy_ * 0.8f;
  }
}
