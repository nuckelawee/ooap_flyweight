#ifndef PARTICLE_H
#define PARTICLE_H

#include <QColor>
#include <QString>

enum class ParticleShape {
  Circle,
  Square,
  Triangle
};

class Particle {
 public:
  Particle(float x, float y, float vx, float vy,
           const QColor& color, float radius, ParticleShape shape);

  void Update(float delta_time, float width, float height);

  // Getters
  float x() const { return x_; }
  float y() const { return y_; }
  float vx() const { return vx_; }
  float vy() const { return vy_; }
  QColor color() const { return color_; }
  float radius() const { return radius_; }
  ParticleShape shape() const { return shape_; }

 private:
  // Extrinsic state (unique per particle) - позиция и скорость
  float x_, y_;
  float vx_, vy_;

  // Intrinsic state (could be shared) - цвет, размер, форма
  // В версии БЕЗ flyweight каждая частица хранит эти данные
  QColor color_;
  float radius_;
  ParticleShape shape_;

  // Physics constants
  static constexpr float kGravity = 200.0f;
  static constexpr float kDamping = 0.98f;
};

#endif  // PARTICLE_H
