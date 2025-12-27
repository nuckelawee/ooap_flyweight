#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>
#include "particle_type.h"

class Particle {
public:
    // С паттерном Flyweight: принимаем shared_ptr на ParticleType
    Particle(float x, float y, float vx, float vy,
             std::shared_ptr<ParticleType> type);

    void update(float delta_time, float width, float height);

    // Getters for extrinsic state
    float x() const { return _x; }
    float y() const { return _y; }
    float vx() const { return _vx; }
    float vy() const { return _vy; }

    // Getters for intrinsic state (через shared ParticleType)
    QString imagePath() const { return _type->imagePath(); }
    float radius() const { return _type->radius(); }

    std::shared_ptr<ParticleType> type() const { return _type; }

    // Setters for velocity
    void setVx(float vx) { _vx = vx; }
    void setVy(float vy) { _vy = vy; }

private:
    // Extrinsic state (unique per particle) - только позиция и скорость!
    float _x, _y;
    float _vx, _vy;

    // Intrinsic state (shared via Flyweight pattern)
    std::shared_ptr<ParticleType> _type;
};

#endif  // PARTICLE_H
