#include "particle.h"
#include <QImage>

// Physics constants
constexpr float kGravity = 200.0f;
constexpr float kDamping = 0.98f;

Particle::Particle(float x, float y, float vx, float vy,
                   const QString& image_path, float radius)
    : _x(x), _y(y), _vx(vx), _vy(vy),
      _imagePath(image_path), _radius(radius),
      _imageWidth(0), _imageHeight(0) {
    // БЕЗ Flyweight: КАЖДАЯ частица загружает, масштабирует и КОПИРУЕТ пиксели!
    // std::vector<uint8_t> НЕ имеет implicit sharing — это реальная глубокая копия!

    // Высокое внутреннее разрешение — чтобы данные были достаточно тяжёлыми
    // и разница с Flyweight-версией была видна по FPS
    // 128x128 RGBA = 64 КБ на частицу, 5000 частиц = ~312 МБ!
    constexpr int kInternalSize = 128;

    QImage original_image(image_path);
    if (original_image.isNull()) {
        // Заглушка — красный квадрат
        QImage fallback(kInternalSize, kInternalSize, QImage::Format_RGBA8888);
        fallback.fill(Qt::red);
        _imageWidth = fallback.width();
        _imageHeight = fallback.height();
        const uint8_t* bits = fallback.constBits();
        _pixelData.assign(bits, bits + fallback.sizeInBytes());
    } else {
        QImage scaled = original_image.scaled(
            kInternalSize, kInternalSize,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        ).convertToFormat(QImage::Format_RGBA8888);

        _imageWidth = scaled.width();
        _imageHeight = scaled.height();
        const uint8_t* bits = scaled.constBits();
        // Глубокая копия пиксельных данных в вектор — каждая частица получает свою копию!
        _pixelData.assign(bits, bits + scaled.sizeInBytes());
    }
}

void Particle::update(float delta_time, float width, float height) {
    // Apply gravity
    _vy += kGravity * delta_time;

    // Update position
    _x += _vx * delta_time;
    _y += _vy * delta_time;

    // Apply damping
    _vx *= kDamping;
    _vy *= kDamping;

    // Collision with walls
    if (_x - _radius < 0) {
        _x = _radius;
        _vx = -_vx * 0.8f;
    } else if (_x + _radius > width) {
        _x = width - _radius;
        _vx = -_vx * 0.8f;
    }

    if (_y - _radius < 0) {
        _y = _radius;
        _vy = -_vy * 0.8f;
    } else if (_y + _radius > height) {
        _y = height - _radius;
        _vy = -_vy * 0.8f;
    }
}
