#ifndef PARTICLE_TYPE_FACTORY_H
#define PARTICLE_TYPE_FACTORY_H

#include <map>
#include <memory>
#include <string>
#include <QString>
#include "particle_type.h"

// Flyweight Factory - управляет созданием и хранением shared ParticleType объектов
// Гарантирует, что для каждой уникальной комбинации параметров существует только один объект
class ParticleTypeFactory {
public:
    // Получить ParticleType (создаст новый только если такой комбинации ещё нет)
    std::shared_ptr<ParticleType> getParticleType(const QString& image_path,
                                                   float radius);

    // Получить все созданные ParticleType объекты
    std::vector<std::shared_ptr<ParticleType>> getAllTypes() const;

    // Статистика
    int getTypeCount() const { return _particleTypes.size(); }

    // Singleton instance
    static ParticleTypeFactory& instance();

private:
    ParticleTypeFactory() = default;

    // Генерирует уникальный ключ для комбинации параметров
    std::string generateKey(const QString& image_path, float radius) const;

    // Хранилище всех созданных ParticleType объектов
    // Ключ - уникальная строка из параметров, значение - shared_ptr на ParticleType
    std::map<std::string, std::shared_ptr<ParticleType>> _particleTypes;
};

#endif  // PARTICLE_TYPE_FACTORY_H
