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

  // Predefined particle types (intrinsic state)
  struct ParticleType {
    QColor color;
    float radius;
    ParticleShape shape;
  };

  std::vector<ParticleType> types = {
    {QColor(220, 50, 50), 8.0f, ParticleShape::Circle},      // Dark red circles
    {QColor(50, 180, 50), 6.0f, ParticleShape::Square},      // Dark green squares
    {QColor(50, 50, 220), 10.0f, ParticleShape::Triangle},   // Dark blue triangles
    {QColor(200, 180, 0), 7.0f, ParticleShape::Circle},      // Dark yellow circles
    {QColor(180, 50, 180), 9.0f, ParticleShape::Square},     // Dark magenta squares
  };

  for (int i = 0; i < count; ++i) {
    // Spawn particles across the entire canvas area
    float x = 50.0f + rng->generateDouble() * (canvas_width_ - 100.0f);
    float y = 50.0f + rng->generateDouble() * (canvas_height_ - 100.0f);
    float vx = -100.0f + rng->generateDouble() * 200.0f;
    float vy = -200.0f + rng->generateDouble() * 100.0f;  // Start with some upward velocity

    // Random type
    const auto& type = types[rng->bounded(static_cast<int>(types.size()))];

    // ВАЖНО: Без flyweight каждая частица создает свою копию всех данных
    particles_.push_back(std::make_unique<Particle>(
      x, y, vx, vy, type.color, type.radius, type.shape
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

QVariantList ParticleSystem::getParticleData() {
  QVariantList data;
  for (const auto& particle : particles_) {
    QVariantMap particleData;
    particleData["x"] = particle->x();
    particleData["y"] = particle->y();
    particleData["radius"] = particle->radius();
    particleData["color"] = particle->color();
    particleData["shape"] = static_cast<int>(particle->shape());
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
