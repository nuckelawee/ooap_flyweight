#include "particle_type_factory.h"
#include <sstream>
#include <iomanip>

ParticleTypeFactory& ParticleTypeFactory::Instance() {
  static ParticleTypeFactory instance;
  return instance;
}

std::shared_ptr<ParticleType> ParticleTypeFactory::GetParticleType(
    const QColor& color, float radius, ParticleShape shape) {
  // Генерируем уникальный ключ для этой комбинации параметров
  std::string key = GenerateKey(color, radius, shape);

  // Проверяем, существует ли уже такой тип
  auto it = particle_types_.find(key);
  if (it != particle_types_.end()) {
    // Возвращаем существующий объект (FLYWEIGHT!)
    return it->second;
  }

  // Создаём новый ParticleType только если такой комбинации ещё не было
  auto new_type = std::make_shared<ParticleType>(color, radius, shape);
  particle_types_[key] = new_type;
  return new_type;
}

std::vector<std::shared_ptr<ParticleType>> ParticleTypeFactory::GetAllTypes() const {
  std::vector<std::shared_ptr<ParticleType>> types;
  for (const auto& pair : particle_types_) {
    types.push_back(pair.second);
  }
  return types;
}

std::string ParticleTypeFactory::GenerateKey(const QColor& color,
                                              float radius,
                                              ParticleShape shape) const {
  std::ostringstream oss;
  oss << color.red() << "_"
      << color.green() << "_"
      << color.blue() << "_"
      << std::fixed << std::setprecision(2) << radius << "_"
      << static_cast<int>(shape);
  return oss.str();
}
