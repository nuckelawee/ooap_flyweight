#include "particle_type_factory.h"
#include <sstream>
#include <iomanip>

ParticleTypeFactory& ParticleTypeFactory::instance() {
    static ParticleTypeFactory instance;
    return instance;
}

std::shared_ptr<ParticleType> ParticleTypeFactory::getParticleType(
    const QString& image_path, float radius) {
    // Генерируем уникальный ключ для этой комбинации параметров
    std::string key = generateKey(image_path, radius);

    // Проверяем, существует ли уже такой тип
    auto it = _particleTypes.find(key);
    if (it != _particleTypes.end()) {
        // Возвращаем существующий объект (FLYWEIGHT!)
        return it->second;
    }

    // Создаём новый ParticleType только если такой комбинации ещё не было
    auto new_type = std::make_shared<ParticleType>(image_path, radius);
    _particleTypes[key] = new_type;
    return new_type;
}

std::vector<std::shared_ptr<ParticleType>> ParticleTypeFactory::getAllTypes() const {
    std::vector<std::shared_ptr<ParticleType>> types;
    for (const auto& pair : _particleTypes) {
        types.push_back(pair.second);
    }
    return types;
}

std::string ParticleTypeFactory::generateKey(const QString& image_path,
                                             float radius) const {
    std::ostringstream oss;
    oss << image_path.toStdString() << "_"
        << std::fixed << std::setprecision(2) << radius;
    return oss.str();
}
