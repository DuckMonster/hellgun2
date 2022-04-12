#pragma once
#include "fx/particlesystem.h"

class Muzzle_Flash_System : public Particle_System
{
public:
	void init(const Vec3& direction);
};