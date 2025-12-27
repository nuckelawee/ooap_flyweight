#include "particle_renderer.h"
#include "particle_system.h"
#include "particle_type_factory.h"
#include <QPainter>
#include <QPainterPath>

ParticleRenderer::ParticleRenderer(QQuickItem* parent)
    : QQuickPaintedItem(parent), particle_system_(nullptr) {
  setRenderTarget(QQuickPaintedItem::FramebufferObject);
  setPerformanceHint(QQuickPaintedItem::FastFBOResizing);
}

void ParticleRenderer::setParticleSystem(ParticleSystem* system) {
  if (particle_system_ == system) {
    return;
  }

  particle_system_ = system;
  ClearPixmapCache();  // Очищаем кэш при смене системы
  emit particleSystemChanged();
}

void ParticleRenderer::paint(QPainter* painter) {
  if (!particle_system_) {
    return;
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->fillRect(0, 0, width(), height(), QColor("#f5f5f5"));

  // ПРЯМОЙ доступ к частицам - без копирования в QVariantList!
  const auto& particles = particle_system_->particles();

  for (const auto& particle : particles) {
    // Получаем ParticleType
    QColor color = particle->color();
    float radius = particle->radius();
    ParticleShape shape = particle->shape();

    // Создаём временный ParticleType для поиска в кэше
    // (в идеале надо получить указатель на реальный ParticleType из частицы)
    auto particle_type = ParticleTypeFactory::Instance().GetParticleType(color, radius, shape);

    // Получаем QPixmap из кэша (или создаём новый)
    QPixmap pixmap = GetParticleTypePixmap(particle_type.get());

    // Быстрая отрисовка: просто копируем готовый QPixmap!
    painter->drawPixmap(
      QPointF(particle->x() - radius - 2, particle->y() - radius - 2),
      pixmap
    );
  }
}

QPixmap ParticleRenderer::GetParticleTypePixmap(const ParticleType* type) {
  // Проверяем кэш
  auto it = pixmap_cache_.find(type);
  if (it != pixmap_cache_.end()) {
    return it->second;
  }

  // Создаём новый QPixmap для этого ParticleType (ОДИН РАЗ!)
  float radius = type->radius();
  int size = static_cast<int>(radius * 2 + 4);  // +4 для stroke

  QPixmap pixmap(size, size);
  pixmap.fill(Qt::transparent);

  QPainter pixmap_painter(&pixmap);
  pixmap_painter.setRenderHint(QPainter::Antialiasing, true);

  QColor color = type->color();
  pixmap_painter.setBrush(color);
  pixmap_painter.setPen(QPen(color.darker(150), 2));

  float center = size / 2.0f;

  switch (type->shape()) {
    case ParticleShape::Circle:
      pixmap_painter.drawEllipse(QPointF(center, center), radius, radius);
      break;

    case ParticleShape::Square:
      pixmap_painter.drawRect(QRectF(center - radius, center - radius, radius * 2, radius * 2));
      break;

    case ParticleShape::Triangle: {
      QPainterPath path;
      path.moveTo(center, center - radius);
      path.lineTo(center - radius, center + radius);
      path.lineTo(center + radius, center + radius);
      path.closeSubpath();
      pixmap_painter.drawPath(path);
      break;
    }
  }

  // Сохраняем в кэш
  pixmap_cache_[type] = pixmap;

  return pixmap;
}

void ParticleRenderer::ClearPixmapCache() {
  pixmap_cache_.clear();
}
