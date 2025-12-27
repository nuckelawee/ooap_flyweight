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
    // ОПТИМИЗАЦИЯ: Прямой доступ к ParticleType БЕЗ lookup в фабрике!
    const ParticleType* particle_type = particle->type();
    float radius = particle_type->radius();

    // Получаем QPixmap из кэша (или создаём новый) - только ОДИН lookup!
    QPixmap pixmap = GetParticleTypePixmap(particle_type);

    // Быстрая отрисовка: просто копируем готовый QPixmap!
    painter->drawPixmap(
      QPointF(particle->x() - radius, particle->y() - radius),
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

  // Загружаем изображение из файла и масштабируем
  QString image_path = type->image_path();
  float radius = type->radius();
  int size = static_cast<int>(radius * 2);

  QPixmap original_pixmap(image_path);
  if (original_pixmap.isNull()) {
    // Если изображение не загрузилось, создаём красный квадрат-заглушку
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::red);
    pixmap_cache_[type] = pixmap;
    return pixmap;
  }

  // ОПТИМИЗАЦИЯ: Используем FastTransformation - быстрее чем SmoothTransformation
  // для небольших изображений разница в качестве незаметна
  QPixmap scaled_pixmap = original_pixmap.scaled(
    size, size,
    Qt::KeepAspectRatio,
    Qt::FastTransformation
  );

  // Сохраняем в кэш
  pixmap_cache_[type] = scaled_pixmap;

  return scaled_pixmap;
}

void ParticleRenderer::ClearPixmapCache() {
  pixmap_cache_.clear();
}
