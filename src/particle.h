#ifndef PARTICLE_H
#define PARTICLE_H

#include <QString>

class Particle {
 public:
  Particle(float x, float y, float vx, float vy,
           const QString& image_path, float radius);

  void Update(float delta_time, float width, float height);

  // Getters
  float x() const { return x_; }
  float y() const { return y_; }
  float vx() const { return vx_; }
  float vy() const { return vy_; }
  QString image_path() const { return image_path_; }
  float radius() const { return radius_; }

  // Setters for velocity
  void set_vx(float vx) { vx_ = vx; }
  void set_vy(float vy) { vy_ = vy; }

 private:
  // Extrinsic state (unique per particle) - позиция и скорость
  float x_, y_;
  float vx_, vy_;

  // Intrinsic state (could be shared) - путь к изображению и размер
  // В версии БЕЗ flyweight каждая частица хранит эти данные (дублирование!)
  QString image_path_;
  float radius_;

  // Physics constants
  static constexpr float kGravity = 200.0f;
  static constexpr float kDamping = 0.98f;
};

#endif  // PARTICLE_H
