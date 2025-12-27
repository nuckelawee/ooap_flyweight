#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <QQuickPaintedItem>
#include "particle_system.h"

class ParticleRenderer : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(ParticleSystem* particleSystem READ particleSystem WRITE setParticleSystem NOTIFY particleSystemChanged)

public:
    explicit ParticleRenderer(QQuickItem* parent = nullptr);

    void paint(QPainter* painter) override;

    ParticleSystem* particleSystem() const { return _particleSystem; }
    void setParticleSystem(ParticleSystem* system);

signals:
    void particleSystemChanged();

private:
    ParticleSystem* _particleSystem;
};

#endif  // PARTICLE_RENDERER_H
