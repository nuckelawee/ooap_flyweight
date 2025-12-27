#ifndef PARTICLE_TYPE_H
#define PARTICLE_TYPE_H

#include <QString>

// Flyweight class - хранит intrinsic state (разделяемое состояние)
// Один объект ParticleType может использоваться тысячами частиц
class ParticleType {
 public:
  ParticleType(const QString& image_path, float radius);

  // Getters для intrinsic state
  QString image_path() const { return image_path_; }
  float radius() const { return radius_; }

 private:
  // Intrinsic state - данные, которые разделяются между множеством частиц
  QString image_path_;  // Путь к файлу изображения
  float radius_;        // Размер частицы
};

#endif  // PARTICLE_TYPE_H
