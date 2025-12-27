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
  Q_PROPERTY(double memoryUsageMB READ memoryUsageMB NOTIFY memoryUsageMBChanged)
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
  Q_INVOKABLE QVariantList getParticleTypes();  // Получить все уникальные ParticleType

  int particleCount() const { return particles_.size(); }
  double fps() const { return current_fps_; }
  double memoryUsageMB() const { return memory_usage_mb_; }
  int objectCount() const;  // С flyweight = количество ParticleType объектов

  // Прямой доступ к частицам для ParticleRenderer (без копирования!)
  const std::vector<std::unique_ptr<Particle>>& particles() const { return particles_; }

 signals:
  void particleCountChanged();
  void fpsChanged();
  void memoryUsageMBChanged();
  void objectCountChanged();

 private slots:
  void Update();

 private:
  void UpdateMetrics();
  void CalculateMemoryUsage();

  std::vector<std::unique_ptr<Particle>> particles_;
  QTimer* update_timer_;
  QElapsedTimer elapsed_timer_;

  float canvas_width_;
  float canvas_height_;

  // Metrics
  double current_fps_;
  double memory_usage_mb_;
  qint64 last_frame_time_;
  int frame_count_;
  qint64 fps_update_time_;

  static constexpr int kUpdateIntervalMs = 16;  // ~60 FPS
  static constexpr int kFpsUpdateIntervalMs = 500;  // Update FPS every 500ms
};

#endif  // PARTICLE_SYSTEM_H
