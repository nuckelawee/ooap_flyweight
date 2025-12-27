#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <QQuickPaintedItem>
#include <QPixmap>
#include "particle_system.h"

class ParticleRenderer : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(ParticleSystem* particleSystem READ particleSystem WRITE setParticleSystem NOTIFY particleSystemChanged)

public:
  explicit ParticleRenderer(QQuickItem* parent = nullptr);

  void paint(QPainter* painter) override;

  ParticleSystem* particleSystem() const { return particle_system_; }
  void setParticleSystem(ParticleSystem* system);

signals:
  void particleSystemChanged();

private:
  QPixmap GetPixmap(const QString& image_path, float radius);

private:
  ParticleSystem* particle_system_;
  // БЕЗ Flyweight: НЕТ кэширования! Каждая частица загружает своё изображение каждый раз!
};

#endif  // PARTICLE_RENDERER_H
