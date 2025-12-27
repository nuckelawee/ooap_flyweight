#include "particle_renderer.h"
#include "particle_system.h"
#include <QPainter>
#include <QImage>

ParticleRenderer::ParticleRenderer(QQuickItem* parent)
    : QQuickPaintedItem(parent), _particleSystem(nullptr) {
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing);
}

void ParticleRenderer::setParticleSystem(ParticleSystem* system) {
    if (_particleSystem == system) {
        return;
    }

    _particleSystem = system;
    emit particleSystemChanged();
}

void ParticleRenderer::paint(QPainter* painter) {
    if (!_particleSystem) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillRect(0, 0, width(), height(), QColor("#f5f5f5"));

    const auto& particles = _particleSystem->particles();

    for (const auto& particle : particles) {
        float radius = particle->radius();
        int w = particle->imageWidth();
        int h = particle->imageHeight();

        if (w <= 0 || h <= 0) {
            continue;
        }

        // Создаём QImage-обёртку из raw данных частицы для отрисовки
        QImage image(particle->pixelData().data(), w, h, QImage::Format_RGBA8888);

        // Рисуем в визуальном размере (radius*2), масштабируя из внутреннего разрешения
        float displaySize = radius * 2;
        QRectF targetRect(particle->x() - radius, particle->y() - radius,
                          displaySize, displaySize);
        painter->drawImage(targetRect, image);
    }
}
