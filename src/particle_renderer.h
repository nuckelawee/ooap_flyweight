#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <QQuickPaintedItem>
#include <QPixmap>
#include <map>
#include <memory>
#include "particle_type.h"
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
  QPixmap GetParticleTypePixmap(const ParticleType* type);
  void ClearPixmapCache();

private:
  ParticleSystem* particle_system_;
  std::map<const ParticleType*, QPixmap> pixmap_cache_;
};

#endif  // PARTICLE_RENDERER_H
