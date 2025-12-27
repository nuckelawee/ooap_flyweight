#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <QObject>
#include <QVariantList>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include <memory>
#include "particle.h"

class ParticleSystem : public QObject {
    Q_OBJECT
    Q_PROPERTY(int particleCount READ particleCount NOTIFY particleCountChanged)
    Q_PROPERTY(double fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int objectCount READ objectCount NOTIFY objectCountChanged)

public:
    explicit ParticleSystem(QObject* parent = nullptr);
    ~ParticleSystem() override;

    Q_INVOKABLE void start(int width, int height);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void addParticles(int count);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void throwUpParticles();
    Q_INVOKABLE QVariantList getParticleData();
    Q_INVOKABLE QVariantList getParticleTypes();

    int particleCount() const { return _particles.size(); }
    double fps() const { return _currentFps; }
    double memoryUsageMB() const { return _memoryUsageMB; }
    int objectCount() const;  // С flyweight = количество ParticleType объектов

    // Прямой доступ к частицам для ParticleRenderer (без копирования!)
    const std::vector<std::unique_ptr<Particle>>& particles() const { return _particles; }

signals:
    void particleCountChanged();
    void fpsChanged();
    void objectCountChanged();

private slots:
    void update();

private:
    void updateMetrics();

    std::vector<std::unique_ptr<Particle>> _particles;
    QTimer* _updateTimer;
    QElapsedTimer _elapsedTimer;

    float _canvasWidth;
    float _canvasHeight;

    // Metrics
    double _currentFps;
    double _memoryUsageMB;
    qint64 _lastFrameTime;
    int _frameCount;
    qint64 _fpsUpdateTime;
};

#endif  // PARTICLE_SYSTEM_H
