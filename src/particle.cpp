#include "particle.h"

// Physics constants
constexpr float kGravity = 200.0f;
constexpr float kDamping = 0.98f;

Particle::Particle(float x, float y, float vx, float vy,
                   std::shared_ptr<ParticleType> type)
    : _x(x), _y(y), _vx(vx), _vy(vy), _type(type) {
}

void Particle::update(float delta_time, float width, float height) {
    // Apply gravity
    _vy += kGravity * delta_time;

    // Update position
    _x += _vx * delta_time;
    _y += _vy * delta_time;

    // Apply damping
    _vx *= kDamping;
    _vy *= kDamping;

    // Collision with walls
    float radius = _type->radius();
    if (_x - radius < 0) {
        _x = radius;
        _vx = -_vx * 0.8f;  // Energy loss on bounce
    } else if (_x + radius > width) {
        _x = width - radius;
        _vx = -_vx * 0.8f;
    }

    if (_y - radius < 0) {
        _y = radius;
        _vy = -_vy * 0.8f;
    } else if (_y + radius > height) {
        _y = height - radius;
        _vy = -_vy * 0.8f;
    }
}
