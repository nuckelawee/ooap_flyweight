#include "particle_system.h"
#include <QRandomGenerator>
#include <QDebug>

ParticleSystem::ParticleSystem(QObject* parent)
    : QObject(parent),
      update_timer_(new QTimer(this)),
      canvas_width_(800),
      canvas_height_(600),
      current_fps_(0.0),
      memory_usage_mb_(0.0),
      last_frame_time_(0),
      frame_count_(0),
      fps_update_time_(0) {
  connect(update_timer_, &QTimer::timeout, this, &ParticleSystem::Update);
}

ParticleSystem::~ParticleSystem() {
  stop();
}

void ParticleSystem::start(int width, int height) {
  canvas_width_ = width;
  canvas_height_ = height;

  if (!elapsed_timer_.isValid()) {
    elapsed_timer_.start();
    last_frame_time_ = elapsed_timer_.elapsed();
    fps_update_time_ = last_frame_time_;
    update_timer_->start(kUpdateIntervalMs);
  }
}

void ParticleSystem::stop() {
  update_timer_->stop();
}

void ParticleSystem::addParticles(int count) {
  QRandomGenerator* rng = QRandomGenerator::global();

  // Predefined particle types - используем картинки из папки data/
  // ВАЖНО: Без flyweight каждая частица будет хранить свою копию QString с путем!
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
    float x = 50.0f + rng->generateDouble() * (canvas_width_ - 100.0f);
    float y = 50.0f + rng->generateDouble() * (canvas_height_ - 100.0f);
    float vx = -100.0f + rng->generateDouble() * 200.0f;
    float vy = -200.0f + rng->generateDouble() * 100.0f;  // Start with some upward velocity

    // Random type
    const auto& type = types[rng->bounded(static_cast<int>(types.size()))];

    // ВАЖНО: Без flyweight каждая частица создает свою копию всех данных (включая QString!)
    particles_.push_back(std::make_unique<Particle>(
      x, y, vx, vy, type.image_path, type.radius
    ));
  }

  CalculateMemoryUsage();
  emit particleCountChanged();
  emit objectCountChanged();
  emit memoryUsageMBChanged();
}

void ParticleSystem::clear() {
  particles_.clear();
  CalculateMemoryUsage();
  emit particleCountChanged();
  emit objectCountChanged();
  emit memoryUsageMBChanged();
}

void ParticleSystem::throwUpParticles() {
  QRandomGenerator* rng = QRandomGenerator::global();

  // Consider particles as "fallen" if they are in the bottom 20% of the canvas
  float threshold_y = canvas_height_ * 0.8f;

  for (auto& particle : particles_) {
    if (particle->y() > threshold_y) {
      // Give particles a strong upward velocity
      float new_vx = -80.0f + rng->generateDouble() * 160.0f;  // Random horizontal velocity
      float new_vy = -300.0f - rng->generateDouble() * 200.0f;  // Strong upward velocity (-300 to -500)

      particle->set_vx(new_vx);
      particle->set_vy(new_vy);
    }
  }
}

QVariantList ParticleSystem::getParticleData() {
  QVariantList data;
  for (const auto& particle : particles_) {
    QVariantMap particleData;
    particleData["x"] = particle->x();
    particleData["y"] = particle->y();
    particleData["radius"] = particle->radius();
    particleData["image_path"] = particle->image_path();
    data.append(particleData);
  }
  return data;
}

void ParticleSystem::Update() {
  qint64 current_time = elapsed_timer_.elapsed();
  float delta_time = (current_time - last_frame_time_) / 1000.0f;
  last_frame_time_ = current_time;

  // Update all particles
  for (auto& particle : particles_) {
    particle->Update(delta_time, canvas_width_, canvas_height_);
  }

  // Update metrics
  UpdateMetrics();
}

void ParticleSystem::UpdateMetrics() {
  frame_count_++;
  qint64 current_time = elapsed_timer_.elapsed();

  if (current_time - fps_update_time_ >= kFpsUpdateIntervalMs) {
    double elapsed_seconds = (current_time - fps_update_time_) / 1000.0;
    current_fps_ = frame_count_ / elapsed_seconds;
    frame_count_ = 0;
    fps_update_time_ = current_time;
    emit fpsChanged();
  }
}

void ParticleSystem::CalculateMemoryUsage() {
  // Approximate memory usage calculation
  // Без flyweight: каждая частица хранит все данные
  size_t particle_size = sizeof(Particle);
  size_t total_bytes = particles_.size() * (particle_size + sizeof(std::unique_ptr<Particle>));

  memory_usage_mb_ = total_bytes / (1024.0 * 1024.0);
}
