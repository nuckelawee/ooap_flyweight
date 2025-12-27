#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>
#include "particle_type.h"

class Particle {
 public:
  // С паттерном Flyweight: принимаем shared_ptr на ParticleType
  Particle(float x, float y, float vx, float vy,
           std::shared_ptr<ParticleType> type);

  void Update(float delta_time, float width, float height);

  // Getters for extrinsic state
  float x() const { return x_; }
  float y() const { return y_; }
  float vx() const { return vx_; }
  float vy() const { return vy_; }

  // Getters for intrinsic state (через shared ParticleType)
  QString image_path() const { return type_->image_path(); }
  float radius() const { return type_->radius(); }

  // ВАЖНО: Прямой доступ к ParticleType для быстрого рендеринга
  const ParticleType* type() const { return type_.get(); }

  // Setters for velocity
  void set_vx(float vx) { vx_ = vx; }
  void set_vy(float vy) { vy_ = vy; }

 private:
  // Extrinsic state (unique per particle) - только позиция и скорость!
  float x_, y_;
  float vx_, vy_;

  // Intrinsic state (shared via Flyweight pattern)
  // ВАЖНО: Это shared_ptr, поэтому тысячи частиц могут указывать на один объект ParticleType!
  std::shared_ptr<ParticleType> type_;

  // Physics constants
  static constexpr float kGravity = 200.0f;
  static constexpr float kDamping = 0.98f;
};

#endif  // PARTICLE_H
