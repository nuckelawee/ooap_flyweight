#ifndef PARTICLE_TYPE_H
#define PARTICLE_TYPE_H

#include <QString>
#include <vector>
#include <cstdint>

// Flyweight class - хранит intrinsic state (разделяемое состояние)
// Один объект ParticleType может использоваться тысячами частиц
class ParticleType {
public:
    ParticleType(const QString& image_path, float radius);

    // Getters для intrinsic state
    QString imagePath() const { return _imagePath; }
    float radius() const { return _radius; }
    const std::vector<uint8_t>& pixelData() const { return _pixelData; }
    int imageWidth() const { return _imageWidth; }
    int imageHeight() const { return _imageHeight; }

private:
    // Intrinsic state - данные, которые разделяются между множеством частиц
    QString _imagePath;
    float _radius;

    // Raw pixel data вместо QPixmap — без implicit sharing!
    // Хранится ОДИН РАЗ в ParticleType и разделяется через shared_ptr (FLYWEIGHT!)
    std::vector<uint8_t> _pixelData;
    int _imageWidth;
    int _imageHeight;
};

#endif  // PARTICLE_TYPE_H
