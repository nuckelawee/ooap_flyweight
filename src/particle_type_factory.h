#ifndef PARTICLE_TYPE_FACTORY_H
#define PARTICLE_TYPE_FACTORY_H

#include <map>
#include <memory>
#include <string>
#include "particle_type.h"

// Flyweight Factory - управляет созданием и хранением shared ParticleType объектов
// Гарантирует, что для каждой уникальной комбинации параметров существует только один объект
class ParticleTypeFactory {
 public:
  // Получить ParticleType (создаст новый только если такой комбинации ещё нет)
  std::shared_ptr<ParticleType> GetParticleType(const QColor& color,
                                                 float radius,
                                                 ParticleShape shape);

  // Статистика
  int GetTypeCount() const { return particle_types_.size(); }

  // Singleton instance
  static ParticleTypeFactory& Instance();

 private:
  ParticleTypeFactory() = default;

  // Генерирует уникальный ключ для комбинации параметров
  std::string GenerateKey(const QColor& color, float radius, ParticleShape shape) const;

  // Хранилище всех созданных ParticleType объектов
  // Ключ - уникальная строка из параметров, значение - shared_ptr на ParticleType
  std::map<std::string, std::shared_ptr<ParticleType>> particle_types_;
};

#endif  // PARTICLE_TYPE_FACTORY_H
