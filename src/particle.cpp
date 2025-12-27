#include "particle.h"
#include <algorithm>

Particle::Particle(float x, float y, float vx, float vy,
                   const QColor& color, float radius, ParticleShape shape)
    : x_(x), y_(y), vx_(vx), vy_(vy),
      color_(color), radius_(radius), shape_(shape) {
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
  if (x_ - radius_ < 0) {
    x_ = radius_;
    vx_ = -vx_ * 0.8f;  // Energy loss on bounce
  } else if (x_ + radius_ > width) {
    x_ = width - radius_;
    vx_ = -vx_ * 0.8f;
  }

  if (y_ - radius_ < 0) {
    y_ = radius_;
    vy_ = -vy_ * 0.8f;
  } else if (y_ + radius_ > height) {
    y_ = height - radius_;
    vy_ = -vy_ * 0.8f;
  }
}
