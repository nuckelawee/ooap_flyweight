#include "particle_renderer.h"
#include "particle_system.h"
#include <QPainter>

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
    // БЕЗ Flyweight: каждая частица хранит свой image_path (дублирование!)
    QString image_path = particle->image_path();
    float radius = particle->radius();

    // Получаем QPixmap из кэша (или создаём новый)
    // Кэшируем по комбинации path+radius вместо указателя на ParticleType
    QPixmap pixmap = GetPixmap(image_path, radius);

    // Быстрая отрисовка: просто копируем готовый QPixmap!
    painter->drawPixmap(
      QPointF(particle->x() - radius, particle->y() - radius),
      pixmap
    );
  }
}

QPixmap ParticleRenderer::GetPixmap(const QString& image_path, float radius) {
  // БЕЗ FLYWEIGHT: КАЖДЫЙ РАЗ загружаем изображение заново!
  // Это ОЧЕНЬ медленно - для каждой частицы из 5000 загружаем файл и масштабируем!

  int size = static_cast<int>(radius * 2);

  QPixmap original_pixmap(image_path);
  if (original_pixmap.isNull()) {
    // Если изображение не загрузилось, создаём красный квадрат-заглушку
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::red);
    return pixmap;
  }

  // МЕДЛЕННАЯ операция: масштабирование для КАЖДОЙ частицы!
  QPixmap scaled_pixmap = original_pixmap.scaled(
    size, size,
    Qt::KeepAspectRatio,
    Qt::SmoothTransformation  // Используем качественное, но медленное масштабирование
  );

  return scaled_pixmap;
}
