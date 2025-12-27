#include "particle_type.h"
#include <QImage>
#include <Qt>

ParticleType::ParticleType(const QString& image_path, float radius)
    : _imagePath(image_path), _radius(radius),
      _imageWidth(0), _imageHeight(0) {
    // Загружаем изображение ОДИН РАЗ при создании ParticleType (Flyweight!)
    // Пиксельные данные хранятся в std::vector — разделяются через shared_ptr

    // Высокое внутреннее разрешение — чтобы данные были достаточно тяжёлыми
    // и разница между Flyweight / без Flyweight была видна по FPS
    constexpr int kInternalSize = 128;

    QImage original_image(image_path);
    if (original_image.isNull()) {
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
        _pixelData.assign(bits, bits + scaled.sizeInBytes());
    }
}
