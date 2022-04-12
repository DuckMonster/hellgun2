#pragma once
#include "particlesystem.h"

class Surface_Impact_System : public Particle_System
{
public:
	void init(const Vec3& normal, const Vec3& enter_direction, const Vec3& exit_direction, float scale = 1.f);
};