#ifndef PARTICLE_TYPE_H
#define PARTICLE_TYPE_H

#include <QColor>

enum class ParticleShape {
  Circle,
  Square,
  Triangle
};

// Flyweight class - хранит intrinsic state (разделяемое состояние)
// Один объект ParticleType может использоваться тысячами частиц
class ParticleType {
 public:
  ParticleType(const QColor& color, float radius, ParticleShape shape);

  // Getters для intrinsic state
  QColor color() const { return color_; }
  float radius() const { return radius_; }
  ParticleShape shape() const { return shape_; }

 private:
  // Intrinsic state - данные, которые разделяются между множеством частиц
  QColor color_;
  float radius_;
  ParticleShape shape_;
};

#endif  // PARTICLE_TYPE_H
