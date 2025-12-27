#include "particle_system.h"
#include <QRandomGenerator>
#include <QDebug>

constexpr int kUpdateIntervalMs = 16;  // ~60 FPS
constexpr int kFpsUpdateIntervalMs = 500;  // Update FPS every 500ms

ParticleSystem::ParticleSystem(QObject* parent)
    : QObject(parent),
      _updateTimer(new QTimer(this)),
      _canvasWidth(800),
      _canvasHeight(600),
      _currentFps(0.0),
      _memoryUsageMB(0.0),
      _lastFrameTime(0),
      _frameCount(0),
      _fpsUpdateTime(0) {
    connect(_updateTimer, &QTimer::timeout, this, &ParticleSystem::update);
}

ParticleSystem::~ParticleSystem() {
    stop();
}

void ParticleSystem::start(int width, int height) {
    _canvasWidth = width;
    _canvasHeight = height;

    if (!_elapsedTimer.isValid()) {
        _elapsedTimer.start();
        _lastFrameTime = _elapsedTimer.elapsed();
        _fpsUpdateTime = _lastFrameTime;
        _updateTimer->start(kUpdateIntervalMs);
    }
}

void ParticleSystem::stop() {
    _updateTimer->stop();
}

void ParticleSystem::addParticles(int count) {
    QRandomGenerator* rng = QRandomGenerator::global();

    struct ParticleType {
        QString image_path;
        float radius;
    };

    std::vector<ParticleType> types = {
        {"data/blob.png", 15.0f},
        {"data/leaflet.png", 12.0f},
        {"data/snowflake1.png", 18.0f},
        {"data/snowflake2.png", 14.0f},
        {"data/snowflake3.png", 10.0f},
    };

    for (int i = 0; i < count; ++i) {
        // Spawn particles across the entire canvas area
        float x = 50.0f + rng->generateDouble() * (_canvasWidth - 100.0f);
        float y = 50.0f + rng->generateDouble() * (_canvasHeight - 100.0f);
        float vx = -100.0f + rng->generateDouble() * 200.0f;
        float vy = -200.0f + rng->generateDouble() * 100.0f;  // Start with some upward velocity

        // Random type
        const auto& type = types[rng->bounded(static_cast<int>(types.size()))];

        _particles.push_back(std::make_unique<Particle>(
            x, y, vx, vy, type.image_path, type.radius
        ));
    }

    emit particleCountChanged();
    emit objectCountChanged();
}

void ParticleSystem::clear() {
    _particles.clear();
    emit particleCountChanged();
    emit objectCountChanged();
}

void ParticleSystem::throwUpParticles() {
    QRandomGenerator* rng = QRandomGenerator::global();

    // Consider particles as "fallen" if they are in the bottom 20% of the canvas
    float threshold_y = _canvasHeight * 0.8f;

    for (auto& particle : _particles) {
        if (particle->y() > threshold_y) {
            // Give particles a strong upward velocity
            float new_vx = -80.0f + rng->generateDouble() * 160.0f;  // Random horizontal velocity
            float new_vy = -300.0f - rng->generateDouble() * 200.0f;  // Strong upward velocity (-300 to -500)

            particle->setVx(new_vx);
            particle->setVy(new_vy);
        }
    }
}

QVariantList ParticleSystem::getParticleData() {
    QVariantList data;
    for (const auto& particle : _particles) {
        QVariantMap particleData;
        particleData["x"] = particle->x();
        particleData["y"] = particle->y();
        particleData["radius"] = particle->radius();
        particleData["image_path"] = particle->imagePath();
        data.append(particleData);
    }
    return data;
}

void ParticleSystem::update() {
    qint64 current_time = _elapsedTimer.elapsed();
    float delta_time = (current_time - _lastFrameTime) / 1000.0f;
    _lastFrameTime = current_time;

    // Update all particles
    for (auto& particle : _particles) {
        particle->update(delta_time, _canvasWidth, _canvasHeight);
    }

    // Update metrics
    updateMetrics();
}

void ParticleSystem::updateMetrics() {
    _frameCount++;
    qint64 current_time = _elapsedTimer.elapsed();

    if (current_time - _fpsUpdateTime >= kFpsUpdateIntervalMs) {
        double elapsed_seconds = (current_time - _fpsUpdateTime) / 1000.0;
        _currentFps = _frameCount / elapsed_seconds;
        _frameCount = 0;
        _fpsUpdateTime = current_time;
        emit fpsChanged();
    }
}
