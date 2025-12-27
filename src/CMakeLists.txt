target_sources(appparticles PRIVATE
    particle.h
    particle.cpp
    particle_type.h
    particle_type.cpp
    particle_type_factory.h
    particle_type_factory.cpp
    particle_system.h
    particle_system.cpp
    particle_renderer.h
    particle_renderer.cpp
)

target_include_directories(appparticles PRIVATE .)
    
