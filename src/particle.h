#ifndef PARTICLE_H
#define PARTICLE_H

#include <QString>
#include <vector>
#include <cstdint>

class Particle {
public:
    Particle(float x, float y, float vx, float vy,
             const QString& image_path, float radius);

    void update(float delta_time, float width, float height);

    // Getters
    float x() const { return _x; }
    float y() const { return _y; }
    float vx() const { return _vx; }
    float vy() const { return _vy; }
    QString imagePath() const { return _imagePath; }
    float radius() const { return _radius; }
    const std::vector<uint8_t>& pixelData() const { return _pixelData; }
    int imageWidth() const { return _imageWidth; }
    int imageHeight() const { return _imageHeight; }

    // Setters for velocity
    void setVx(float vx) { _vx = vx; }
    void setVy(float vy) { _vy = vy; }

private:
    // Extrinsic state (unique per particle) - позиция и скорость
    float _x, _y;
    float _vx, _vy;

    QString _imagePath;
    float _radius;

    // Raw pixel data — БЕЗ implicit sharing!
    // Каждая частица хранит свою ПОЛНУЮ копию пиксельных данных
    std::vector<uint8_t> _pixelData;
    int _imageWidth;
    int _imageHeight;
};

#endif  // PARTICLE_H
